//
// Created by andy on 10/12/20.
//

#include <client/game/client.h>

int main() {
    Client client;
    client.start();
    while(client.isRunning()) {}
    client.stop();
    client.join();
}
