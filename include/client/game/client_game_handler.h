//
// Created by andy on 27/11/20.
//

#ifndef TP_WOLFENSTEIN_CLIENT_GAME_HANDLER_H
#define TP_WOLFENSTEIN_CLIENT_GAME_HANDLER_H

#include <SDL.h>
#include "client/graphics/ray_caster.h"
#include "client/graphics/sdl_window.h"
#include "client/graphics/sdl_texture.h"
#include "client/sdl_exception.h"
#include "client_map.h"
#include "sdl_audio_player.h"
#include "client/graphics/game_screen.h"
#include "client/communication/client_parser.h"
#include "client_event_handler.h"
#include "event_generator.h"
#include "change_processor.h"
#include <vector>
#include <common/change.h>
#include <common/shared_queue.h>
#include <common/map_parser.h>
#include <common/config_parser.h>
#include <client/sounds/audio_manager.h>
#include "client_map_generator.h"
#include "client_player_initializer.h"
#include "off_game_change_processor.h"

class ClientGameHandler {
public:
    ClientGameHandler(int map_width, int map_height,
                      SharedQueue<Change>& change_queue,
                      BlockingQueue<Event>& event_queue);
    void start();
    void killPlayer();
    void respawnPlayer();
    bool isRunning();
private:
    ClientPlayer player = ClientPlayer("Player 1");
    std::atomic<bool> running;
    ClientMap map;
    EventGenerator event_generator;
    ObjectInfoProvider info_provider;
    GameScreen screen;
    ClientParser client_parser;
    ClientEventHandler event_handler;
    OffGameChangeProcessor off_game_change_processor;
    ChangeProcessor change_processor;
    std::string map_path;
    AudioManager audio_manager;
    ClientPlayerInitializer player_initializer;
    std::atomic<bool> game_started;
    void displayIntro();
    int displayMatchModeMenu();
    void displayLevelSelectionMenu();
    void setMapPath(int chosen_map);
    void initializePlayer();
    void initializeMap();
    void displayLoadingScreen();

};


#endif //TP_WOLFENSTEIN_CLIENT_GAME_HANDLER_H