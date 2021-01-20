#ifndef TP_WOLFENSTEIN_GAME_HANDLER_H
#define TP_WOLFENSTEIN_GAME_HANDLER_H

#include <string>
#include "common/event_queue.h"
#include "common/event_handler.h"
#include "game.h"
#include "common/event_processor.h"
#include "common/shared_queue.h"
#include "common/blocking_queue.h"
#include "common/change.h"

class GameHandler {
private:
    Game game;
    SharedQueue<Event> sharedQueue;
    //BlockingQueue<Change> blockingQueue;
    //std::vector<ClientUpdater>
    EventQueue eq;
    EventHandler eh;
    EventProcessor ep;
public:
    GameHandler(std::string path);
    void run();

};


#endif //TP_WOLFENSTEIN_GAME_HANDLER_H
