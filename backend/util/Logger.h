#pragma once
#include <iostream>
#include <string>
#include <mutex>

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
public:
    static void log(LogLevel level, const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string prefix;
        switch (level) {
        case LogLevel::Debug: prefix = "[DEBUG] "; break;
        case LogLevel::Info:  prefix = "[INFO ] "; break;
        case LogLevel::Warn:  prefix = "[WARN ] "; break;
        case LogLevel::Error: prefix = "[ERROR] "; break;
        }
        std::cout << prefix << msg << std::endl;
    }

    static void debug(const std::string& msg) { log(LogLevel::Debug, msg); }
    static void info(const std::string& msg) { log(LogLevel::Info, msg); }
    static void warn(const std::string& msg) { log(LogLevel::Warn, msg); }
    static void error(const std::string& msg) { log(LogLevel::Error, msg); }

    static void debug(const Board& board) {
        debug(board.debugString());
    }

    static void debug(const Bitboard& bb) {
        debug(bb.debugString());
    }

private:
    static inline std::mutex mutex_;
};
