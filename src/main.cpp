/**
 * @file main.cpp
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Главный файл точки входа программы сервера vcalc
 */

#include "Server.h"

/**
 * @brief Главная функция программы
 * @param argc Количество аргументов командной строки
 * @param argv Массив строк аргументов командной строки
 * @return 0 при успешном завершении, ненулевой код ошибки в противном случае
 */
int main(int argc, char* argv[]) {
    Server server;
    return server.start(argc, argv);
}
