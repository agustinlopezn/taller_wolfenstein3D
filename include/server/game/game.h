#ifndef TP_WOLFENSTEIN_GAME_H
#define TP_WOLFENSTEIN_GAME_H

#include <string>
#include "common/thread.h"
#include <atomic>
#include "colission_handler.h"
#include "pickup_handler.h"
#include "player.h"
#include "config_parser.h"

class Game {

private:
    std::string map_path;
    std::atomic<bool> playing;
    Map map;
    std::string config_path;
    PickUpHandler ph;
    ColissionHandler ch;
    std::vector<Player> players;

    //ShootHandler sh;

public:
    Game(std::string _map_path, std::string _config_path);
    void movePlayer(int id, double angle);
    void shoot(int id, double angle);
    ~Game();

    void show();
};

#endif //TP_WOLFENSTEIN_GAME_H