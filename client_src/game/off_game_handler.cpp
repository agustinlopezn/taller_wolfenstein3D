#include <SDL_events.h>
#include <SDL_timer.h>
#include "client/game/off_game_handler.h"
#include "client_routes.h"

OffGameHandler::OffGameHandler(GameScreen& _screen,
                               ClientPlayer& player,
                               ClientMap& _map,
                               SharedQueue<Change>& change_queue,
                               BlockingQueue<Event>& event_queue,
                               bool& _skip_stats,
                               bool& _game_running) :
    screen(_screen),
    map(_map),
    change_processor(_map, player, change_queue, game_started, player_ready,
                     server_down),
    event_generator(event_queue, player),
    skip_stats(_skip_stats),
    game_running(_game_running) {
}

void OffGameHandler::handleOffGame(const std::string& map_name) {
  initializeMap(map_name);
  displayLoadingScreen();
}

void OffGameHandler::displayLoadingScreen() {
  screen.displayLoadingScreen(true);
  SDL_Event event;
  while (!player_ready || !game_started) {
    usleep(30000);
    change_processor.processOffGameChanges();
    if (SDL_PollEvent(&event) == 0)
      continue;
    bool player_pressed_p = handleLoadingScreenEvent(event);
    if (player_pressed_p) {
      event_generator.generateReadyEventIfNecessary();
      screen.displayLoadingScreen(false);
    }
    if (skip_stats) {
      game_running = false;
      return;
    }
    if (server_down) {
      screen.displayNetworkConnectionErrorScreen();
      sleep(2);
      game_running = false;
      skip_stats = true;
      return;
    }
  }
}

void OffGameHandler::initializeMap(const std::string& map_name) {
  std::string map_path = MAPS_PATH_FOLDER + map_name;
  MapParser map_parser(map_path);
  ClientMapGenerator::create(map, map_parser);
}

int OffGameHandler::handleLoadingScreenEvent(SDL_Event event) {
  if (event.type == SDL_QUIT) {
    skip_stats = true;
    return 0;
  }
  if (event.type != SDL_KEYDOWN)
    return 0;
  auto& key_event = (SDL_KeyboardEvent&) event;
  if (key_event.keysym.sym == SDLK_p)
    return 1;
  return 0;
}


