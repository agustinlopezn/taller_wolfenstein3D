#include "server/game/game.h"
#include <iostream>

#define MAX_PLAYERS 8
#define MAX_DOOR_OPEN 5
#define TURN_ANGLE (M_PI/20)

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

Game::Game(const std::string& map_path, std::string config_path, BotsManager& bm, int _players_requested) :
           mapParser(map_path),
           mapGenerator(mapParser, MAX_PLAYERS, config_path),
           map(mapGenerator.create()),
           configParser(config_path),
           colHandler(map),
           blockingItemHandler(map),
           shootHandler(map, scoreHandler),
           pickUpHandler(config_path, scoreHandler),
           dropHandler(config_path, map),
           botsManager(bm),
           players_requested(_players_requested){
}

Game::~Game() { botsManager.destroyBots(); }

/* RECEIVED EVENTS */

std::pair<int,std::unordered_map<Coordinate, Positionable, Coordinate::HashFunction>> Game::connectPlayer() {
    std::unique_lock<std::mutex> lock(m);
    Player player(players_ids,
                  configParser.getSpecificCategory("player", "max_bullets"),
                  configParser.getSpecificCategory("player", "max_hp"),
                  configParser.getSpecificCategory("player", "bullets"),
                  configParser.getSpecificCategory("player", "max_lives"),
                  configParser);
    map.addPlayer(players_ids);
    players.push_back(player);
    players_ids++;
    players_alive++;
    std::cout << "Player " << player.getID() << " connected to game.\n";
    std::unordered_map<Coordinate, Positionable, Coordinate::HashFunction> board = map.getBoard();
    return std::make_pair(player.getID(), board);
}

double Game::getAngleToMove(int direction) {
    switch (direction) {
        case (MOVE_LEFT): { return M_PI/2; }
        case (MOVE_RIGHT): { return -M_PI/2; }
        case (MOVE_DOWN): { return M_PI; }
        default: break;
    }
    return 0;
}

std::pair<Coordinate, std::vector<Positionable>> Game::movePlayer(int id, int move_direction) {
    std::vector<Positionable> erased_positionables;
    Player& player = players[id];
    double angle = player.getAngle() + getAngleToMove(move_direction);
    Coordinate old_pos = map.getPlayerPosition(player.getID());
    Coordinate new_pos = colHandler.moveToPosition(old_pos, angle);

    std::vector<std::pair<Coordinate, Positionable>> items = colHandler.getCloseItems(old_pos, new_pos);
    for (auto& item : items) {
        if (pickUpHandler.pickUp(item.second, player)) {
            map.erasePositionableAt(item.first);
            erased_positionables.push_back(item.second);
        }
    }
    map.setPlayerPosition(player.getID(), new_pos);
    return std::make_pair(new_pos, erased_positionables);
}

std::pair<Hit, std::vector<Change>> Game::shoot(int id) {
    Player& shooter = players[id];
    double angle = shooter.getAngle();
    std::pair<Hit, std::vector<Change>> hit_event = shootHandler.shoot(shooter, angle, players);
    if (hit_event.first.playerDied()) playerDies(hit_event.first);
    return hit_event;
}

std::pair<bool, int> Game::openDoor(int id) {
    /* El bool indica si uso llave, el int el id de la puerta */
    Coordinate door_to_open = colHandler.getCloseBlocking(map.getPlayerPosition(id),
                                                          players[id].getAngle(), "door");
    /* No hay puertas cerca */
    if (!door_to_open.isValid()) return std::make_pair(false, -1);

    int player_keys_before = players[id].getKeys();
    /* No tengo llave para abrir la puerta */
    int opened_door = blockingItemHandler.openDoor(door_to_open, players[id]);
    if (opened_door == -1) return std::make_pair(false, -1);

    /* Exito al abrir la puerta (estaba abierta o gaste llave) */
    doors_to_close[map.getNormalizedCoordinate(door_to_open)] = MAX_DOOR_OPEN;
    bool player_use_key = (player_keys_before == players[id].getKeys());
    return std::make_pair(player_use_key,map.getBlockingItemAt(door_to_open).getId());
}

int Game::pushWall(int id) {
    Coordinate wall_to_push = colHandler.getCloseBlocking(map.getPlayerPosition(id),
                                                          players[id].getAngle(), "wall");
    /* No hay pared cerca */
    if (!wall_to_push.isValid()) return -1;

    /* No hay pared falsa en esa posicion */
    if (!blockingItemHandler.pushWall(wall_to_push)) return -1;

    /* Exito, hay pared falsa, devuelvo ID */
    return map.getBlockingItemAt(wall_to_push).getId();
}

void Game::rotate(int id, int rotation) {
    Player& player = players[id];
    player.addAngle(rotation * TURN_ANGLE);
}

int Game::changeGun(int id, int hotkey) { return players[id].changeGun(hotkey); }

void Game::playerIsReady(int id) {
    std::unique_lock<std::mutex> lock(m);
    players_ready.insert(id);
}

/* GAME CHECK */

bool Game::isNotOver() {
    std::unique_lock<std::mutex> lock(m);
    if (players_alive <= 1) return false;
    /* Se termina por tiempo */
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - time_start;
    if (elapsed_seconds.count()  >= 600) return false; // debe ser >= minutos que dura el game
    return true;
}

int Game::getPlayersAlive() {
    std::unique_lock<std::mutex> lock(m);
    return players_alive;
}

bool Game::isReady() {
    std::unique_lock<std::mutex> lock(m);
    time_start = std::chrono::system_clock::now();
    return (players_alive == players_requested+1 || players_ready.size() == players_requested);
}

/* GAME CHANGERS */

void Game::playerDies(Hit& hit) {
    std::unique_lock<std::mutex> lock(m);
    std::vector<std::pair<int, bool>> dead_respawn_players; //(id, muere y respawnea o no) (para clientes)
    for (auto& dead_player : hit.getDeadPlayers()) {
        if (players[dead_player].dieAndRespawn()) {
            std::pair<std::string, bool> drops = players[dead_player].getDropsFromDeath();
            addDropsToHitEvent(drops, hit, map.getPlayerPosition(dead_player));
            // carga todos los drops de la muerte del player enemigo para enviar a clientes

            map.respawnPlayer(dead_player); // respawnea en el sv
            dead_respawn_players.emplace_back(dead_player, true);
        }
        else {
            map.removePlayer(dead_player); // le pone pos = -1,-1 en el sv
            players_alive--;
            dead_respawn_players.emplace_back(dead_player, false);
        }
    }
    dropHandler.processDrops(hit.getDrops());
    // carga en el mapa del sv esos drops
    hit.setPlayersDeaths(dead_respawn_players);
}

void Game::addDropsToHitEvent(const std::pair<std::string, bool> &drops,
                              Hit &hit, const Coordinate& pos) {
    /* Par: tipo de arma, si tiene llave para dropear o no */
    if (drops.first != "pistol") {
        Coordinate gun_pos(pos.x, pos.y - 2);
        hit.addDrops(drops.first, gun_pos, map.getGlobalID(), GUN_DROP);
    }
    if (drops.second) {
        Coordinate key_pos(pos.x, pos.y + 2);
        hit.addDrops("key", key_pos, map.getGlobalID(), KEY_DROP);
    }
    Coordinate bullets_pos(pos.x + 2, pos.y);
    hit.addDrops("bullets", bullets_pos, map.getGlobalID(), BULLETS_DROP);
}

void Game::closeDoors(std::vector<Change>& changes) {
    for (auto &door : doors_to_close) {
        if (doors_to_close[door.first] == 0) {
            if (!map.isAPlayerInACell(door.first)) {
                int id = map.getGlobalID();
                map.putPositionableAt(Positionable("door", "unlocked_door", id, true), door.first);
                doors_to_close.erase(door.first);
                changes.emplace_back(ADD_UNLOCKED_DOOR, id, door.first.x, door.first.y, true);
            }
        } else {
            doors_to_close[door.first]--;
        }
    }
}

std::vector<Change> Game::passTime() {
    std::vector<Change> changes;
    closeDoors(changes);
    //std::cout << "PASSING TIME\n";
    Hit rpg_explosions = shootHandler.travelAndExplodeAllRPGS(players, changes);
    if (rpg_explosions.playerDied()) playerDies(rpg_explosions);
    hitHandler.processHit(rpg_explosions, changes, getPlayersAlive());
    return changes;
}

/* GAME PRINT */

void Game::show() { map.show(); }

std::vector<std::pair<int,int>> Game::getTop(const std::string& type, int n) {
    if (type == "kills") return scoreHandler.getTopFraggers(n);
    if (type == "bullets") return scoreHandler.getTopShooters(n);
    return scoreHandler.getTopCollectors(n);
}

/* LUA SCRIPT */

void Game::addBot() {
    std::pair<int, std::unordered_map<Coordinate, Positionable, Coordinate::HashFunction>> data = connectPlayer();
    botsManager.addBot(players[data.first], TURN_ANGLE);
}

void Game::releaseBots() {
    botsManager.releaseBots(map, players);
}

