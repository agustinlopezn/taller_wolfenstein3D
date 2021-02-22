#ifndef TP_WOLFENSTEIN_CHANGES_H
#define TP_WOLFENSTEIN_CHANGES_H

#define INVALID 0
#define REMOVE_POSITIONABLE 1
#define MOVE_PLAYER 2
#define CHANGE_POINTS 3
#define CHANGE_HP 4
#define CHANGE_AMMO 5
#define CHANGE_WEAPON 6
#define CHANGE_KEY 7
#define KILL_PLAYER 8  // MORIR DEFINITIVAMENTE
#define RESPAWN_PLAYER 9 // RESPAWNEA PLAYER EN SU SPAWN
#define ADD_PLAYER 10 // ENVIA EL ID DEL PLAYER
#define ADD_BLOOD_PUDDLE_AT 11
#define ADD_BULLETS_AT 12
#define ADD_KEY_AT 13
#define ADD_MACHINE_GUN_AT 14
#define ADD_CHAIN_GUN_AT 15
#define ADD_RPG_GUN_AT 16
#define ADD_UNLOCKED_DOOR 17
#define RPG_MOVE_TO 18
#define RPG_EXPLODE_AT 19

#define MAP_INITIALIZER 20
#define TOP_KILLER 21
#define TOP_SHOOTER 22
#define TOP_SCORER 23
#define TOTAL_PLAYERS_CONNECTED 24 // ENVIA EL TOTAL DE JUGADORES ACTIVOS (INCLUYE AL QUE ENTRO ULTIMO)
#define GAME_START 25
#define GAME_OVER 26
#define GAME_OVER_NETWORK_ERROR 27

// CAMBIOS AUTO GENERADOS POR EL CLIENTE

#define CL_UPDATE_DIRECTION 28

#endif //TP_WOLFENSTEIN_CHANGES_H
