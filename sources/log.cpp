#include "log.h"

std::string getId() {
    boost::thread::id th_id = boost::this_thread::get_id();
    std::stringstream id_str;
    id_str << th_id;

    return id_str.str();
}

void init()
{
    auto sinkFile = boost::log::add_file_log(
        keywords::file_name = "logs/log_%N.log",
        keywords::rotation_size = 128 * 1024 * 1024,
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >
                ("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
    sinkFile->set_filter(
        logging::trivial::severity >= logging::trivial::trace
    );

    auto sinkConsole = logging::add_console_log(
        std::cout,
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >
                ("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
    sinkConsole->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );

    logging::add_common_attributes();
}

void logInfoClientsMessage(const std::string& data) {

    std::string id  = getId();

    std::cout << std::endl;
    BOOST_LOG_TRIVIAL(info) << "THREAD " << std::setw(3)
    << "#" + id << ": [Clients's Message - " << data << "]";

}

void logInfoServersAnswer(const std::string& data) {

    std::string id  = getId();

    std::cout << std::endl;
    BOOST_LOG_TRIVIAL(info) << "THREAD " << std::setw(3)
    << "#" + id << ": [Server's Answer - " << data << "]";

}
