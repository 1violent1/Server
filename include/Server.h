/**
 * @file Server.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл главного управляющего класса Server
 */

#pragma once

#include "Communicator.h"
#include "UserDatabase.h"
#include "Authenticator.h"
#include "DataProcessor.h"
#include "Logger.h"
#include <string>
#include <atomic>

/**
 * @class Server
 * @brief Главный класс сервера, управляющий жизненным циклом и серверным циклом обработки запросов
 */
class Server {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Server();

    /**
     * @brief Деструктор класса
     */
    ~Server() = default;

    /**
     * @brief Запуск сервера с аргументами командной строки
     * @param argc Количество аргументов
     * @param argv Массив строк аргументов
     * @return 0 при успешном завершении, ненулевой код при критической ошибке
     */
    int start(int argc, char* argv[]);

    /**
     * @brief Вывод справки по параметрам запуска программы
     * @param programName Имя исполняемого файла
     */
    static void printHelp(const char* programName);

    /**
     * @brief Остановка сервера при получении системного сигнала (Ctrl+C)
     */
    static void stop();

    /**
     * @brief Закрытие сетевых сокетов для мгновенного прерывания ожидания клиентов
     */
    void stopServer();

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

    /**
     * @brief Разбор аргументов командной строки
     * @param argc Количество аргументов
     * @param argv Массив аргументов
     * @return 0 — успешный разбор (продолжить), 1 — запрошена справка (выход с кодом 0), 2 — ошибка параметров (выход с кодом 1)
     */
    int parseCommandLine(int argc, char* argv[]);

    /**
     * @brief Обработка сеанса связи с одним подключенным клиентом
     */
    void handleClient();
};
