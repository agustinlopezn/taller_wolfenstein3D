//
// Created by andy on 27/11/20.
//

#ifndef TP_WOLFENSTEIN_CLIENT_GAME_H
#define TP_WOLFENSTEIN_CLIENT_GAME_H

#include <SDL.h>
#include "client/graphics/ray_caster.h"
#include "client/graphics/sdl_window.h"
#include "client/graphics/sdl_texture.h"
#include "client/sdl_exception.h"
#include "client_map.h"
#include "sdl_audio_player.h"
#include "client/graphics/game_screen.h"
#include "client/communication/client_parser.h"
#include "in_game_event_handler.h"
#include "in_game_event_generator.h"
#include "in_game_change_processor.h"
#include <vector>
#include <common/change.h>
#include <common/shared_queue.h>
#include <common/map_parser.h>
#include <common/config_parser.h>
#include <client/sounds/audio_manager.h>
#include "client_map_generator.h"
#include "client_player_initializer.h"
#include "off_game_change_processor.h"
#include "off_game_handler.h"

class ClientGame {
private:
    GameScreen screen;
    ClientMap map;
    ClientPlayer player = ClientPlayer("Player 1");
    AudioManager audio_manager;
    std::atomic<bool> running;
    InGameEventGenerator event_generator;
    ClientParser client_parser;
    InGameEventHandler event_handler;
    InGameChangeProcessor change_processor;
    bool game_started;
    bool player_ready;
    OffGameHandler off_game_handler;
    ClientPlayerInitializer player_initializer;
public:
    ClientGame(SharedQueue<Change>& change_queue,
               BlockingQueue<Event>& event_queue);
    void startGame();
    void killPlayer();
    void respawnPlayer();
    bool isRunning();
    void initializePlayer();
    void initializeMap();
};


#endif //TP_WOLFENSTEIN_CLIENT_GAME_H
