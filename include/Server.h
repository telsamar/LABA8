#pragma once

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "log.h"
#include "Client.h"

namespace ba = boost::asio;
namespace ip = boost::asio::ip;

class Server {
public:
    Server();

    ~Server() = default;

    void work();

    void acceptClientsThread();

    void handleClientsThread();

    void workWithClient(std::shared_ptr<Client>& client);

    void loginClient(ip::tcp::socket& socket, std::string& name);

    void tellClient(ip::tcp::socket& socket, std::string message);

    void listenToClient(ip::tcp::socket& socket, std::string& msg);

    friend void checkConnect(std::shared_ptr<Client>& client,
                             const std::string& command);

private:
    std::vector<std::shared_ptr<Client>> clients_;

    std::unique_ptr<ip::tcp::acceptor> acceptor_;

    ba::io_service service_;

    std::unique_ptr<std::thread> accept;

    std::unique_ptr<std::thread> handle;

    std::vector<std::shared_ptr<std::thread>> clientsThreads;
};
