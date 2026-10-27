#pragma once

#include <vector>
#include <cstdint>

class DataProcessor {
public:
    DataProcessor() = default;

    ~DataProcessor() = default;

    int32_t calculateAverage(const std::vector<int32_t>& data);
};
