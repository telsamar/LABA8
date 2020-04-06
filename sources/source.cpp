// Copyright 2018 Your Name <your_email>

#include <random>
#include <chrono>

std::chrono::seconds sleepDuration(){
    static std::random_device rd;
    static std::mt19937  gen(rd());
    static std::uniform_int_distribution<> timeDist(1, 7);
    return std::chrono::seconds(timeDist(gen));
}

#include <header.hpp>
int main(){
    boost::asio::io_context ioContext;
    while (true) {
        boost::system::error_code error;
        tcp::resolver resolver(ioContext);
        auto endpoints = resolver.resolve("localhost", "60013", error);
        if (error) {
            return 1;
        }
        tcp::socket socket(ioContext);
        boost::asio::connect(socket, endpoints, error);
        if (error) {
            return 2;
        }
        boost::asio::streambuf output;
        boost::asio::streambuf input;
        std::ostream os(&output);
        std::istream is(&input);
        os << "login hello\n";
        boost::asio::write(socket, output, error);
        std::this_thread::sleep_for(sleepDuration());
        std::string message;
        while (error != boost::asio::error::eof){
            is.clear();
            boost::asio::read_until(socket, input, "\n", error);
            if (error){
                return 3;
            }
            message = "";
            is >> message;
            os.clear();
            if (message == "client_list_change" || message == "login_ok") {
                os << "clients\n";
            } else {
                os << "ping\n";
            }
            boost::asio::write(socket, output, error);
            std::this_thread::sleep_for(sleepDuration());
        }
    }
}
