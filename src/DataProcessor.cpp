#include "DataProcessor.h"

int32_t DataProcessor::calculateAverage(const std::vector<int32_t>& data) {
    if (data.empty()) {
        return 0;
    }

    __int128 sum = 0;
    for (int32_t val : data) {
        sum += val;
    }

    __int128 count = static_cast<__int128>(data.size());
    __int128 avg = sum / count;

    constexpr int64_t MAX_LIMIT = 2147483647LL;   //  2^31 - 1
    constexpr int64_t MIN_LIMIT = -2147483648LL;  // -2^31

    if (avg > MAX_LIMIT) {
        return static_cast<int32_t>(MAX_LIMIT);
    }
    if (avg < MIN_LIMIT) {
        return static_cast<int32_t>(MIN_LIMIT);
    }

    return static_cast<int32_t>(avg);
}
