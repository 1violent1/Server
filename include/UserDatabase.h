/**
 * @file UserDatabase.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса UserDatabase для работы с базой учетных записей
 */

#pragma once

#include <string>
#include <unordered_map>

/**
 * @class UserDatabase
 * @brief Класс загрузки и хранения базы пользователей формата LOGIN:PASSWORD
 */
class UserDatabase {
public:
    /**
     * @brief Конструктор по умолчанию
     * @param dbPath Путь к файлу конфигурации базы пользователей (по умолчанию ~/base/vcalc.conf)
     */
    explicit UserDatabase(const std::string& dbPath = "~/base/vcalc.conf");

    /**
     * @brief Загрузка базы пользователей из файла, заданного при инициализации
     * @return true в случае успешной загрузки, false в случае ошибки чтения
     */
    bool load();

    /**
     * @brief Загрузка базы пользователей из указанного файла
     * @param dbPath Путь к файлу конфигурации базы
     * @return true в случае успешной загрузки, false в случае ошибки чтения
     */
    bool load(const std::string& dbPath);

    /**
     * @brief Получение пароля пользователя по его идентификатору (логину)
     * @param login Идентификатор пользователя
     * @return Пароль пользователя
     * @throw std::runtime_error если пользователь не найден
     */
    std::string getPassword(const std::string& login) const;

    /**
     * @brief Проверка существования пользователя в базе
     * @param login Идентификатор пользователя
     * @return true если пользователь существует, иначе false
     */
    bool hasUser(const std::string& login) const;

    /**
     * @brief Получение текущего пути к файлу базы
     * @return Строка пути
     */
    std::string getDbPath() const;

private:
    std::string m_dbPath;
    std::unordered_map<std::string, std::string> m_users;

    /**
     * @brief Преобразование пути с тильдой '~' в абсолютный путь
     * @param path Исходный путь
     * @return Абсолютный путь
     */
    static std::string expandTilde(const std::string& path);
};
