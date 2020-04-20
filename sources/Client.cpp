#include "Client.h"

Client::Client() {
    std::cout << "Enter your name: ";
    std::cin >> this->username_;
    logInfoClientsMessage(this->username_);

    this->socket_.reset(new ip::tcp::socket{this->service_});
}

Client::Client(int x) {
    this->socket_.reset(new ip::tcp::socket{this->service_});
}

Client::Client(const Client& client) {
    this->endpoint_ = client.endpoint_;
}

Client& Client::operator=(const Client& client) {
    *this = client;
    return *this;
}

void Client::work() {
    this->socket_.get()->connect(this->endpoint_);

    this->login();

    std::thread talk(&Client::talkWithServer, this);
    talk.join();
}

void Client::talkWithServer() {
    while (true) {
        std::string message;
        std::string s_count;
        std::string tilda = "\n~ ";
        std::cout << tilda;

        this->tellServer();

        this->listenToServer(s_count);
        int count = std::stoi(s_count);
        if (count) {
            tellServer("ok");
        }

        for (unsigned i = 0; i < count; i++) {
            this->listenToServer(message);
            logInfoServersAnswer(message);
        }
    }
}

void Client::login() {
    std::string message;

    this->tellServer(this->username_);
    this->listenToServer(message);

    std::cout << message << std::endl;
}

void Client::listenToServer(std::string& message) {
    char data[512];

    this->service_.run();
    size_t read_bytes = this->socket_.get()->read_some(ba::buffer(data));
    for (unsigned int i = 0; i < read_bytes - 1; i++) {
        message += data[i];
    }
    this->service_.stop();
}

void Client::tellServer() {
    this->service_.run();

    std::string command;
    std::cin >> command;
    command += '\n';

    this->socket_.get()->write_some(ba::buffer(command));
    this->service_.stop();
}

void Client::tellServer(const std::string& message) {
    this->service_.run();
    this->socket_.get()->write_some(ba::buffer(message + '\n'));
    this->service_.stop();
}

void Client::setUsername(const std::string& username) {
    this->username_ = username;
}

std::string Client::getUsername() {
    return this->username_;
}

void Client::setIsTimeOut() {
    this->isTimeOut_ = true;
    logInfoClientsMessage("Connection Aborted");
}

bool Client::isTimeOut() {
    return this->isTimeOut_;
}

ip::tcp::socket& Client::sock() {
    return *this->socket_.get();
}

void Client::setHasConsole() {
    this->hasConsole_ = true;
}

bool Client::hasConsole() {
    return this->hasConsole_;
}
