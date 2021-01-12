#include "server/entities/gun.h"
#include <iostream>

Gun::Gun(std::string type, int id, int _bullets_per_spray,
         double _fire_rate, double _precision) :
         Positionable("gun", type, id, false),
         bullets_per_spray(_bullets_per_spray),
         fire_rate(_fire_rate), 
         precision(_precision) {
}

int Gun::getDamage() {return damage;} // falta calcular bien el resto

double Gun::getPrecision() { return precision; }

Gun::~Gun() {}

int Gun::getRange() {return 15;}



Gun::Gun(const Gun& other) {
    this->bullets_per_spray = other.bullets_per_spray;
    this->fire_rate = other.fire_rate;
    this->precision = other.precision;
    this->damage = other.damage;
    this->category = other.category;
    this->type = other.type;
    this->id = other.id;
    this->is_blocking = other.is_blocking;
}
Gun& Gun::operator=(const Gun& other) {
    this->bullets_per_spray = other.bullets_per_spray;
    this->fire_rate = other.fire_rate;
    this->precision = other.precision;
    this->damage = other.damage;
    this->category = other.category;
    this->type = other.type;
    this->id = other.id;
    this->is_blocking = other.is_blocking;
    return *this;
}



