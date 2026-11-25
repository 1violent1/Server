/**
 * @file Logger.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса Logger для ведения журнала работы сервера
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * @class Logger
 * @brief Класс для записи информационных сообщений и ошибок в файл журнала
 */
class Logger {
public:
    /**
     * @brief Конструктор по умолчанию
     * @param logPath Путь к файлу журнала (по умолчанию ~/log/vcalc.log)
     */
    explicit Logger(const std::string& logPath = "~/log/vcalc.log");

    /**
     * @brief Деструктор класса, закрывает файл журнала
     */
    ~Logger();

    /**
     * @brief Установка пути к файлу журнала и его открытие
     * @param logPath Путь к файлу журнала
     */
    void setLogPath(const std::string& logPath);

    /**
     * @brief Запись сообщения об ошибке или событии в файл журнала
     * @param message Текст сообщения
     * @param isCritical Флаг критичности ошибки (true — критическая, false — некритическая)
     */
    void logError(const std::string& message, bool isCritical = false);

    /**
     * @brief Преобразование пути с тильдой '~' в абсолютный путь домашней директории
     * @param path Исходный путь
     * @return Абсолютный путь
     */
    static std::string expandTilde(const std::string& path);

private:
    std::string m_logPath;
    std::ofstream m_logFile;
    std::mutex m_mutex;

    /**
     * @brief Получение текущего системного времени в виде форматированной строки
     * @return Строка формата ГГГГ-ММ-ДД ЧЧ:ММ:СС
     */
    static std::string getCurrentTime();
};
