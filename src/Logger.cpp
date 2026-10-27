#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <filesystem>

Logger::Logger(const std::string& logPath) {
    setLogPath(logPath);
}

Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

std::string Logger::expandTilde(const std::string& path) {
    if (path.empty() || path[0] != '~') {
        return path;
    }
    const char* home = std::getenv("HOME");
    if (!home) {
        return path;
    }
    return std::string(home) + path.substr(1);
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

void Logger::setLogPath(const std::string& logPath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logPath = expandTilde(logPath);

    if (m_logFile.is_open()) {
        m_logFile.close();
    }

    try {
        std::filesystem::path p(m_logPath);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }
        m_logFile.open(m_logPath, std::ios::app);
    } catch (...) {
    }
}

void Logger::logError(const std::string& message, bool isCritical) {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::string level = isCritical ? "CRITICAL" : "ERROR";
    std::string record = "[" + getCurrentTime() + "] [" + level + "] " + message;

    if (m_logFile.is_open()) {
        m_logFile << record << std::endl;
    } else {
        std::cerr << record << std::endl;
    }
}
