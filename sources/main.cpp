#include "Server.h"
#include "Client.h"

int main(int argc, char* argv[]) {
    init();

    Server server;

    Client client;

    client.work();
}
