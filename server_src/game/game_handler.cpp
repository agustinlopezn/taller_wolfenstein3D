#include "server/game/game_handler.h"

GameHandler::GameHandler(std::string path) : game(path, path), eh(eq), ep(eq) {}

void GameHandler::run() {
    //eh.start();
    //ep.start();
    //proccess();
}
/*
void GameHandler::proccess() {
    while (game.isNotOver()) {
        while (eq.isNotEmpty()) {
            Event event = eq.pop();
            Change change = ep.proccess(event);
            notify_clients(change);
        }
        sleep(20);
    }
    stop();
}
*/
