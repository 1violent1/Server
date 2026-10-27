#pragma once

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    explicit Logger(const std::string& logPath = "~/log/vcalc.log");

    ~Logger();

    void setLogPath(const std::string& logPath);

    void logError(const std::string& message, bool isCritical = false);

    static std::string expandTilde(const std::string& path);

private:
    std::string m_logPath;
    std::ofstream m_logFile;
    std::mutex m_mutex;

    static std::string getCurrentTime();
};
