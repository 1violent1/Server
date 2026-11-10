#include <UnitTest++/UnitTest++.h>
#include "Logger.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <filesystem>

struct LoggerFixture {
    std::string tempLogFile = "/tmp/test_vcalc_logger.log";
    std::string tempNestedLog = "/tmp/test_vcalc_dir/nested/test.log";

    LoggerFixture() {
        std::remove(tempLogFile.c_str());
        std::filesystem::remove_all("/tmp/test_vcalc_dir");
    }

    ~LoggerFixture() {
        std::remove(tempLogFile.c_str());
        std::filesystem::remove_all("/tmp/test_vcalc_dir");
    }

    bool fileContains(const std::string& path, const std::string& pattern) {
        std::ifstream in(path);
        if (!in.is_open()) return false;
        std::string line;
        while (std::getline(in, line)) {
            if (line.find(pattern) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
};

SUITE(LoggerTest) {
    TEST_FIXTURE(LoggerFixture, ExpandTilde) {
        const char* home = std::getenv("HOME");
        if (home) {
            std::string expanded = Logger::expandTilde("~/log/test.log");
            std::string expected = std::string(home) + "/log/test.log";
            CHECK_EQUAL(expected, expanded);
        }
        CHECK_EQUAL("/var/log/test.log", Logger::expandTilde("/var/log/test.log"));
    }

    TEST_FIXTURE(LoggerFixture, WriteNonCriticalError) {
        {
            Logger logger(tempLogFile);
            logger.logError("Non-critical test warning", false);
        }
        CHECK(fileContains(tempLogFile, "[ERROR]"));
        CHECK(fileContains(tempLogFile, "Non-critical test warning"));
    }

    TEST_FIXTURE(LoggerFixture, WriteCriticalError) {
        {
            Logger logger(tempLogFile);
            logger.logError("Fatal system alert", true);
        }
        CHECK(fileContains(tempLogFile, "[CRITICAL]"));
        CHECK(fileContains(tempLogFile, "Fatal system alert"));
    }

    TEST_FIXTURE(LoggerFixture, AutoCreatesParentDirectories) {
        {
            Logger logger(tempNestedLog);
            logger.logError("Nested dir entry", false);
        }
        CHECK(std::filesystem::exists(tempNestedLog));
        CHECK(fileContains(tempNestedLog, "Nested dir entry"));
    }
}

