#include "server/game/positionable.h"

Positionable::Positionable(std::string _category, std::string _type,
                           int _id, bool _is_blocking) :
                           category(_category), type(_type), id(_id),
                           is_blocking(_is_blocking) {}

std::string Positionable::getType() {
    return type;
}

int Positionable::getId() {
    return id;
}

std::string Positionable::getCategory() {
    return category;
}

bool Positionable::isBlocking() const {
    return is_blocking;
}

Positionable::Positionable(const Positionable& other) {
    this->category = other.category;
    this->type = other.type;
    this->id = other.id;
    this->is_blocking = other.is_blocking;
}
Positionable& Positionable::operator=(const Positionable& other) {
    this->category = other.category;
    this->type = other.type;
    this->id = other.id;
    this->is_blocking = other.is_blocking;
    return *this;
}
