#include "server/events/client_updater.h"
#include <unistd.h>

ClientUpdater::ClientUpdater(int fd, int id) :
                            skt(fd),
                            change_queue(Change()),
                            player_id(id),
                            alive(true) {
    std::cout << "CONSTRUCTOR DEL UPDATER" << skt.file_descriptor <<  " - PLAYER " << player_id << "\n";
}

ClientUpdater::~ClientUpdater() {}

void ClientUpdater::run() {
    while (alive) {
        Change change = change_queue.pop();
        if (change.isGlobal() || change.getPlayerID() == player_id) {
            std::cout << "PLAYER " << player_id << " -> Popie un change de id: " << change.getChangeID() << "\n";
            std::cout << "EN EL RUN DEL UPDATER " << skt.file_descriptor << " - PLAYER " << player_id << "\n";
            skt.sendMsg(change.serialize());
        }
    }
}

void ClientUpdater::update(Change change) {change_queue.push(change);}


void ClientUpdater::stop() {
    alive = false;
    change_queue.close();
    //socket.closeSocket();
}

int ClientUpdater::getPlayerId() const {
    return player_id;
}
