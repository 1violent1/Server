#pragma once

#include "Communicator.h"
#include "UserDatabase.h"
#include "Authenticator.h"
#include "DataProcessor.h"
#include "Logger.h"
#include <string>
#include <atomic>

class Server {
public:
    Server();

    ~Server() = default;

    int start(int argc, char* argv[]);

    static void printHelp(const char* programName);

    static void stop();

private:
    Communicator m_communicator;
    UserDatabase m_database;
    Authenticator m_authenticator;
    DataProcessor m_processor;
    Logger m_logger;

    std::string m_dbPath;
    std::string m_logPath;
    int m_port;

    static std::atomic<bool> s_running;

    int parseCommandLine(int argc, char* argv[]);

    void handleClient();
};
