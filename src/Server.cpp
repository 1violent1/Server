#include "Server.h"
#include <iostream>
#include <getopt.h>
#include <csignal>
#include <cstdlib>

std::atomic<bool> Server::s_running(true);

static void signalHandler(int signum) {
    (void)signum;
    Server::stop();
}

Server::Server()
    : m_dbPath("~/base/vcalc.conf"),
      m_logPath("~/log/vcalc.log"),
      m_port(33333) {}

void Server::stop() {
    s_running = false;
}

void Server::printHelp(const char* programName) {
    std::cout << "Сервер для клиент-серверной системы обработки данных (vcalc)\n\n"
              << "Использование:\n"
              << "  " << programName << " [-b <база>] [-l <журнал>] [-p <порт>] [-h]\n\n"
              << "Параметры:\n"
              << "  -b <путь>   Путь к файлу базы пользователей (по умолчанию: ~/base/vcalc.conf)\n"
              << "  -l <путь>   Путь к файлу журнала работы (по умолчанию: ~/log/vcalc.log)\n"
              << "  -p <порт>   Номер сетевого порта (по умолчанию: 33333)\n"
              << "  -h          Вывод этой справки и завершение работы\n\n"
              << "При запуске без параметров автоматически выводится данная справка.\n";
}

int Server::parseCommandLine(int argc, char* argv[]) {
    if (argc == 1) {
        printHelp(argv[0]);
        return 1;
    }

    int opt = 0;
    while ((opt = getopt(argc, argv, "b:l:p:h")) != -1) {
        switch (opt) {
            case 'b':
                m_dbPath = optarg;
                break;
            case 'l':
                m_logPath = optarg;
                break;
            case 'p': {
                try {
                    int p = std::stoi(optarg);
                    if (p < 1 || p > 65535) {
                        std::cerr << "Ошибка: некорректный номер порта: " << optarg << std::endl;
                        return 2;
                    }
                    m_port = p;
                } catch (...) {
                    std::cerr << "Ошибка: некорректный номер порта: " << optarg << std::endl;
                    return 2;
                }
                break;
            }
            case 'h':
                printHelp(argv[0]);
                return 1;
            default:
                printHelp(argv[0]);
                return 2;
        }
    }

    return 0;
}

int Server::start(int argc, char* argv[]) {
    int cliStatus = parseCommandLine(argc, argv);
    if (cliStatus == 1) {
        return 0;
    }
    if (cliStatus == 2) {
        return 1;
    }

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGPIPE, SIG_IGN);

    m_logger.setLogPath(m_logPath);

    if (!m_communicator.bindAndListen(m_port)) {
        m_logger.logError("Не удалось привязать сокет к порту " + std::to_string(m_port), true);
        return 1;
    }
    else {
        m_logger.logError("Сокет привязан к порту");
        }

    if (!m_database.load(m_dbPath)) {
        m_logger.logError("Критическая ошибка: не удалось загрузить базу пользователей из: " + m_dbPath, true);
        return 1;
    }
    else {
        m_logger.logError("База успешно загружена.");
        }

    m_logger.logError("Server started on port " + std::to_string(m_port));

    while (s_running) {
        if (m_communicator.acceptConnection()) {
            handleClient();
        }
    }

    m_logger.logError("Server stopped");
    m_communicator.closeServer();
    return 0;
}

void Server::handleClient() {
    std::string login = m_communicator.receiveData();
    if (login.empty()) {
        m_communicator.closeConnection();
        return;
    }

    if (!m_database.hasUser(login)) {
        m_communicator.sendData("ERR");
        m_logger.logError("Auth failed: пользователь не найден (" + login + ")");
        m_communicator.closeConnection();
        return;
    }
    else {
        m_logger.logError("Auth complete: пользователь найден.");
        }

    std::string salt = m_authenticator.generateSalt();

    if (!m_communicator.sendData(salt)) {
        m_logger.logError("Ошибка отправки соли клиенту (" + login + ")");
        m_communicator.closeConnection();
        return;
    }
    else {
        m_logger.logError("Соль отправлена.");
        }

    std::string clientHash = m_communicator.receiveData();
    if (clientHash.empty()) {
        m_logger.logError("Auth failed: клиент не передал хеш (" + login + ")");
        m_communicator.closeConnection();
        return;
    }
    else {
        m_logger.logError("Клиент передал хеш.");
        }

    std::string password = m_database.getPassword(login);
    std::string expectedHash = m_authenticator.calculateMD5(password, salt);

    if (clientHash != expectedHash) {
        m_communicator.sendData("ERR");
        m_logger.logError("Auth failed: неверный пароль (" + login + ")");
        m_communicator.closeConnection();
        return;
    }
    else {
        m_logger.logError("Auth complete: верный пароль.");
        }

    if (!m_communicator.sendData("OK")) {
        m_logger.logError("Ошибка отправки OK клиенту (" + login + ")");
        m_communicator.closeConnection();
        return;
    }

    uint32_t numVectors = 0;
    if (!m_communicator.receiveUint32(numVectors)) {
        m_logger.logError("Ошибка приема количества векторов от клиента (" + login + ")");
        m_communicator.closeConnection();
        return;
    }
    else {
        m_logger.logError("Вектор принят.");
        }

    for (uint32_t i = 0; i < numVectors; ++i) {
        uint32_t vectorSize = 0;
        if (!m_communicator.receiveUint32(vectorSize)) {
            m_logger.logError("Ошибка приема размера вектора " + std::to_string(i + 1) + " (" + login + ")");
            m_communicator.closeConnection();
            return;
        }
        else {
        m_logger.logError("Вектор принят.");
        }

        std::vector<int32_t> vectorData;
        if (!m_communicator.receiveVector(vectorData, vectorSize)) {
            m_logger.logError("Ошибка приема данных вектора " + std::to_string(i + 1) + " (" + login + ")");
            m_communicator.closeConnection();
            return;
        }
        else {
        m_logger.logError("Вектор принят.");
        }

        int32_t result = m_processor.calculateAverage(vectorData);

        if (!m_communicator.sendData(result)) {
            m_logger.logError("Ошибка отправки результата вектора " + std::to_string(i + 1) + " (" + login + ")");
            m_communicator.closeConnection();
            return;
        }
        else {
        m_logger.logError("Результаты вектора отправлены.");
        }
    }

    m_communicator.closeConnection();
}
