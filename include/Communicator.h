/**
 * @file Communicator.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса Communicator для сетевого взаимодействия по протоколу TCP
 */

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

/**
 * @class Communicator
 * @brief Класс для управления сокетами TCP, приема соединений и передачи текстовых и бинарных данных
 */
class Communicator {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Communicator();

    /**
     * @brief Деструктор класса, закрывает активные сокеты
     */
    ~Communicator();

    /**
     * @brief Создание серверного сокета и запуск прослушивания порта
     * @param port Номер сетевого порта
     * @return true в случае успеха, false в случае ошибки
     */
    bool bindAndListen(int port);

    /**
     * @brief Принятие входящего соединения от клиента
     * @return true если соединение принято, false в случае ошибки
     */
    bool acceptConnection();

    /**
     * @brief Прием текстовых данных от клиента (логин, хеш)
     * @return Принятая строка с удаленными завершающими символами \\r и \\n
     */
    std::string receiveData();

    /**
     * @brief Прием фиксированного объема бинарных данных
     * @param buffer Указатель на буфер для записи
     * @param size Необходимое количество байт
     * @return true если все байты успешно прочитаны, иначе false
     */
    bool receiveData(void* buffer, size_t size);

    /**
     * @brief Прием 32-битного беззнакового целого числа
     * @param value Ссылка на переменную для записи
     * @return true в случае успеха, false при ошибке
     */
    bool receiveUint32(uint32_t& value);

    /**
     * @brief Прием вектора 32-битных беззнаковых целых чисел
     * @param vec Вектор для записи данных
     * @param size Количество элементов вектора
     * @return true в случае успеха, false при ошибке
     */
    bool receiveVector(std::vector<uint32_t>& vec, uint32_t size);

    /**
     * @brief Отправка строки клиенту
     * @param str Передаваемая строка
     * @return true в случае успешной отправки, иначе false
     */
    bool sendData(const std::string& str);

    /**
     * @brief Отправка 32-битного беззнакового целого числа (результата вычислений) клиенту
     * @param result Результат вычислений
     * @return true в случае успешной отправки, иначе false
     */
    bool sendData(uint32_t result);

    /**
     * @brief Отправка бинарных данных произвольного размера
     * @param buffer Указатель на данные
     * @param size Количество байт
     * @return true в случае успеха, false при ошибке
     */
    bool sendData(const void* buffer, size_t size);

    /**
     * @brief Закрытие клиентского соединения
     */
    void closeConnection();

    /**
     * @brief Закрытие серверного слушающего сокета
     */
    void closeServer();

private:
    int m_serverSock;
    int m_clientSock;
};
