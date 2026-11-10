#include "DataProcessor.h"

uint32_t DataProcessor::calculateAverage(const std::vector<uint32_t>& data) {
    if (data.empty()) {
        return 0;
    }

    __uint128_t sum = 0;
    for (uint32_t val : data) {
        sum += val;
    }

    __uint128_t count = static_cast<__uint128_t>(data.size());
    __uint128_t avg = sum / count;

    constexpr uint64_t MAX_LIMIT = 4294967295ULL;

    if (avg > MAX_LIMIT) {
        return static_cast<uint32_t>(MAX_LIMIT);
    }

    return static_cast<uint32_t>(avg);
}
