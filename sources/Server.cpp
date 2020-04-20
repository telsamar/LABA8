#include "Server.h"

Server::Server() {
    this->acceptor_.reset(
        new ip::tcp::acceptor(this->service_, ip::tcp::endpoint{
            ip::address::from_string("127.0.0.1"), 8080
        })
    );

    this->work();
}

void Server::work() {
    this->accept.reset(new std::thread(&Server::acceptClientsThread, this));
    this->handle.reset(new std::thread(&Server::handleClientsThread, this));
}

void Server::acceptClientsThread() {

    this->acceptor_.get()->listen();
    while (true) {
        std::shared_ptr<Client> client(new Client(0));

        this->acceptor_.get()->accept(client.get()->sock());

        std::string username;
        this->loginClient(client.get()->sock(), username);
        client.get()->setUsername(username);
        logInfoClientsMessage(username);

        this->tellClient(client.get()->sock(), "login_ok");
        this->clients_.push_back(client);
    }
}

void Server::handleClientsThread() {;
    while (true) {
        for(std::shared_ptr<Client>& client: this->clients_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (!client.get()->isTimeOut() && !client.get()->hasConsole()) {
                std::shared_ptr<std::thread> clientsThread(new std::thread(
                    &Server::workWithClient, this, std::ref(client))
                );
                client.get()->setHasConsole();

                this->clientsThreads.push_back(clientsThread);
            }
        }
    }
}

void checkConnect(std::shared_ptr<Client>& client, const std::string& command) {
    time_t start =  std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );

    while (true) {
        if ((std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()) - start > 5) && command.empty()) {
            client.get()->setIsTimeOut();
            break;
        }
        else if (!command.empty()) {
            break;
        }
    }
}

void Server::workWithClient(std::shared_ptr<Client>& client) {
    while(!client.get()->isTimeOut()) {
        std::string command;

        std::thread check(checkConnect, std::ref(client), std::ref(command));
        this->listenToClient(client.get()->sock(), command);

        if (!client.get()->isTimeOut()) {
            logInfoClientsMessage(command);
            if (command == "list") {
                std::stringstream sstream;
                std::string message;
                std::string s_size;

                int size = this->clients_.size();
                sstream << size;
                sstream >> s_size;

                tellClient(client.get()->sock(), s_size + '\n');
                listenToClient(client.get()->sock(), message);

                if (message == "ok") {
                    for (const auto& cl: this->clients_) {
                        std::string name = cl.get()->getUsername();
                        std::string connect;

                        cl.get()->isTimeOut() ? connect = "list_chaned" :
                                                connect = "ping_ok";

                        std::string clientInfo = name + ": " + connect;
                        tellClient(client.get()->sock(), clientInfo);
                    }
                }
            }
            else {
                tellClient(client.get()->sock(), "1");

                std::string message;
                listenToClient(client.get()->sock(), message);
                if (message == "ok") {
                    tellClient(client.get()->sock(), "ping_ok");
                }
            }
        }
        check.join();
    }
}

void Server::loginClient(ip::tcp::socket& socket, std::string& name) {
    listenToClient(socket, name);
}

void Server::tellClient(ip::tcp::socket& socket, std::string message) {
    this->service_.run();
    socket.write_some(ba::buffer(message + '\n'));
    this->service_.stop();
}

void Server::listenToClient(ip::tcp::socket& socket, std::string& msg) {
    char data[512];

    this->service_.run();
    size_t read_bytes = socket.read_some(ba::buffer(data));
    for (unsigned int i = 0; i < read_bytes - 1; i++) {
        msg += data[i];
    }
    this->service_.stop();
}
