/**
 * @file Authenticator.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса Authenticator для выполнения аутентификации клиента
 */

#pragma once

#include <string>

/**
 * @class Authenticator
 * @brief Класс для генерации криптографической соли и вычисления хеш-функции MD5
 */
class Authenticator {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Authenticator() = default;

    /**
     * @brief Деструктор класса
     */
    ~Authenticator() = default;

    /**
     * @brief Генерация случайной соли SALT16
     * @details Формирует 64-битное случайное число, приведенное к строке из 16 шестнадцатеричных
     * цифр в верхнем регистре (0-9, A-F) с дополнением слева нулями.
     * @return 16-значная строка шестнадцатеричных цифр
     */
    std::string generateSalt();

    /**
     * @brief Вычисление MD5-хеша от конкатенации соли и пароля
     * @details Вычисляет MD5(SALT16 || PASSWORD) и возвращает 32-значную строку
     * шестнадцатеричных цифр в верхнем регистре.
     * @param password Пароль пользователя
     * @param salt Случайная соль SALT16
     * @return 32-значная строка хеша в верхнем регистре
     */
    std::string calculateMD5(const std::string& password, const std::string& salt);
};
