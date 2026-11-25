/**
 * @file DataProcessor.h
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса DataProcessor для обработки числовых векторов
 */

#pragma once

#include <vector>
#include <cstdint>

/**
 * @class DataProcessor
 * @brief Класс для выполнения математических вычислений над векторами данных
 */
class DataProcessor {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    DataProcessor() = default;

    /**
     * @brief Деструктор класса
     */
    ~DataProcessor() = default;

    /**
     * @brief Вычисление среднего арифметического значений вектора
     * @details Вычисляет среднее арифметическое элементов вектора.
     * При переполнении вверх возвращает UINT32_MAX (4294967295).
     * @param data Вектор 32-битных беззнаковых целых чисел (uint32_t)
     * @return 32-битное беззнаковое целое число — результат вычислений
     */
    uint32_t calculateAverage(const std::vector<uint32_t>& data);
};
