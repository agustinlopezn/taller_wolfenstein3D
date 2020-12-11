#ifndef DOOR_H
#define DOOR_H

#include "server/positionable.h"

class Door : public Positionable {
protected:
    bool is_locked;
public:
    Door(std::string object_type, bool _is_locked);
    //virtual open()
    ~Door();
};


#endif // DOOR_H