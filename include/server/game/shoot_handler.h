#ifndef TP_WOLFENSTEIN_SHOOT_HANDLER_H
#define TP_WOLFENSTEIN_SHOOT_HANDLER_H

#include "map.h"

class ShootHandler {
private:
    Map& map;
public:
    ShootHandler(Map& _map) : map(_map) {}

    void shoot(Player& player, double angle, std::vector<Player>& players);

    int hit(Player& player, Player& enemy, int damage);

    std::pair<Coordinate, Coordinate> getAdjacents(Coordinate &pos, double angle);

    bool hitAtPos(Coordinate &pos, std::vector<Player> &players, Player &player,
                  std::vector<std::pair<int, int>> &enemy_dmg_done);
};


#endif //TP_WOLFENSTEIN_SHOOT_HANDLER_H
