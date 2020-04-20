#pragma once

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

std::string getId();

void init();

void logInfoClientsMessage(const std::string& data);

void logInfoServersAnswer(const std::string& data);
