#include <UnitTest++/UnitTest++.h>
#include "DataProcessor.h"
#include <vector>
#include <cstdint>

SUITE(DataProcessorTest) {
    TEST(EmptyVectorReturnsZero) {
        DataProcessor dp;
        std::vector<uint32_t> data;
        CHECK_EQUAL(0u, dp.calculateAverage(data));
    }

    TEST(SingleElement) {
        DataProcessor dp;
        std::vector<uint32_t> data = {42};
        CHECK_EQUAL(42u, dp.calculateAverage(data));
    }

    TEST(SingleMaxElement) {
        DataProcessor dp;
        std::vector<uint32_t> data = {4294967295u};
        CHECK_EQUAL(4294967295u, dp.calculateAverage(data));
    }

    TEST(PositiveNumbersAverage) {
        DataProcessor dp;
        std::vector<uint32_t> data = {10u, 20u, 30u};
        CHECK_EQUAL(20u, dp.calculateAverage(data));
    }

    TEST(IntegerDivisionRoundingTowardsZero) {
        DataProcessor dp;
        std::vector<uint32_t> data = {10u, 11u};
        CHECK_EQUAL(10u, dp.calculateAverage(data));
    }

    TEST(AllZerosAverageZero) {
        DataProcessor dp;
        std::vector<uint32_t> data = {0u, 0u, 0u, 0u, 0u};
        CHECK_EQUAL(0u, dp.calculateAverage(data));
    }

    TEST(PositiveBoundaryValue) {
        DataProcessor dp;
        std::vector<uint32_t> data = {4294967295u, 4294967295u};
        CHECK_EQUAL(4294967295u, dp.calculateAverage(data));
    }

    TEST(LargeNumbersSumAccumulationWithoutOverflow) {
        DataProcessor dp;
        std::vector<uint32_t> data = {3000000000u, 4000000000u};
        CHECK_EQUAL(3500000000u, dp.calculateAverage(data));
    }

    TEST(LargeArraySumAccumulation) {
        DataProcessor dp;
        std::vector<uint32_t> data(10000, 100000u);
        CHECK_EQUAL(100000u, dp.calculateAverage(data));
    }
}

