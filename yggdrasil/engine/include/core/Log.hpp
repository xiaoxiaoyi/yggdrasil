#ifndef _YGG_LOG_HPP
#define _YGG_LOG_HPP

#include <iostream>
#include <sstream>

namespace ygg {

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

template<typename T>
std::string str(T&& value) {
    std::stringstream ss;
    ss << std::forward<T>(value);
    return ss.str();
}

template<typename T, typename... Ts>
std::string str(T&& value, Ts&&... values) {
    return str(value) + str(std::forward<Ts>(values)...);
}

inline void log(LogLevel level, const std::string& message) {
    if(level == LogLevel::INFO) {
        std::clog << "Info: " << message << std::endl;
        return;
    }

    if(level == LogLevel::WARNING) {
        std::cerr << "Warning: " << message << std::endl;
        return;
    }

    if(level == LogLevel::ERROR) {
        std::cerr << "Error: " << message << std::endl;
        exit(-1);
    }
}

}

#endif
