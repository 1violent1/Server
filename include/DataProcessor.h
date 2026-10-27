#pragma once

#include <vector>
#include <cstdint>

class DataProcessor {
public:
    DataProcessor() = default;

    ~DataProcessor() = default;

    uint32_t calculateAverage(const std::vector<uint32_t>& data);
};
