#ifndef TP_WOLFENSTEIN_DROP_H
#define TP_WOLFENSTEIN_DROP_H

#include "coordinate.h"

#define GUN_DROP 0
#define KEY_DROP 1
#define BULLETS_DROP 2

class Drop {
public:
    std::string type;
    int id;
    Coordinate pos;
    int drop_id;

    Drop(std::string _type, int _id, Coordinate _pos, int _drop_id) :
            type(_type), id(_id), pos(_pos), drop_id(_drop_id) {}

    ~Drop() {}
};

#endif //TP_WOLFENSTEIN_DROP_H