#include "server/events/game_handler.h"
#include <unistd.h>

#define MAX_EVENTS 100

GameHandler::GameHandler(std::string map_path,
                         std::string config_path,
                         int _players_n, int _bots_n) :
        eventQueue(Event()),
        botsManager(eventQueue),
        game(map_path, config_path, botsManager),
        eventProcessor(game, config_path),
        alive(true),
        players_n(_players_n),
        bots_n(_bots_n){}

void GameHandler::run() {
    waitInLobby();
    addBots();

    Change change(GAME_START, INVALID, INVALID, INVALID, true);
    clientsManager.notifyClients(change);

    std::cout << "Termino el lobby\n";

    while (game.isNotOver() && alive) {
        int total_events = 0;
        while (total_events < MAX_EVENTS) {
            Event event = eventQueue.pop();
            if (event.isInvalid()) {
                std::cout << "INVALIDOOOO\n";
                break;
            }
            std::cout << "El evento que salio es de: " << event.getEventID() << "\n";
            std::vector<Change> changes = eventProcessor.process(event);
            notifyClients(changes);
            total_events++;
        }
        std::vector<Change> game_changes = game.passTime();
        notifyClients(game_changes);
        game.releaseBots();
        sleep(1);
    }
    std::cout << "Termino la partida!!!!\n";
    sendTops();
}

void GameHandler::notifyClients(std::vector<Change>& changes) {
    for (auto& change : changes) {
        clientsManager.notifyClients(change);
    }
}

void GameHandler::addBots() {
    for (int i = 0; i < bots_n; i++) {
        game.addBot();
    }
    Change change(TOTAL_PLAYERS_CONNECTED, game.getPlayersAlive(), INVALID, INVALID, true);
    clientsManager.notifyClients(change);
}

void GameHandler::waitInLobby() {
    while (!game.isReady() && alive) {
        Event event = eventQueue.pop();
        if (event.isInvalid()) continue;
        if (event.getEventID() != CONNECT_PLAYER && event.getEventID() != PLAYER_READY) continue;
        std::vector<Change> changes = eventProcessor.process(event);
        notifyClients(changes);
    }
}

void GameHandler::notifyTop(std::vector<std::pair<int,int>> top, int change_id) {
    std::vector<Change> changes;
    for (int i = 0; i < top.size(); i++) {
        changes.emplace_back(change_id, top[i].first, top[i].second, i);
    }
    notifyClients(changes);
}

void GameHandler::sendTops() {
    std::vector<std::pair<int,int>> kills = game.getTop("kills", 5);
    std::vector<std::pair<int,int>> bullets = game.getTop("bullets", 5);
    std::vector<std::pair<int,int>> scores = game.getTop("scores", 5);

    notifyTop(kills, TOP_KILLER);
    notifyTop(bullets, TOP_SHOOTER);
    notifyTop(scores, TOP_SCORER);
}

void GameHandler::addNewPlayer(NetworkConnection socket) {
    std::pair<int,std::map<Coordinate, Positionable>> data = game.connectPlayer();
    int id = data.first;
    std::cout << "Player id " << id << std::endl;
    std::map<Coordinate, Positionable> map = data.second;
    clientsManager.addNewPlayer(std::move(socket), id, eventQueue, map);
}

void GameHandler::stop() {
    clientsManager.killPlayers();
    alive = false;
}