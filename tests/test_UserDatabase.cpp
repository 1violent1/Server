#include <UnitTest++/UnitTest++.h>
#include "UserDatabase.h"
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

struct DatabaseFixture {
    std::string tempValidFile = "/tmp/test_vcalc_valid.conf";
    std::string tempEmptyFile = "/tmp/test_vcalc_empty.conf";

    DatabaseFixture() {
        std::ofstream out(tempValidFile);
        out << "# Тестовая база пользователей\n";
        out << "user1:pass123\n";
        out << "user2:secret456\r\n";
        out << "\n";
        out << "admin:superadmin\n";
        out.close();

        std::ofstream emptyOut(tempEmptyFile);
        emptyOut.close();
    }

    ~DatabaseFixture() {
        std::remove(tempValidFile.c_str());
        std::remove(tempEmptyFile.c_str());
    }
};

SUITE(UserDatabaseTest) {
    TEST_FIXTURE(DatabaseFixture, LoadNonExistentFileReturnsFalse) {
        UserDatabase db;
        CHECK(!db.load("/tmp/non_existent_path_987654.conf"));
    }

    TEST_FIXTURE(DatabaseFixture, LoadValidFileReturnsTrue) {
        UserDatabase db;
        CHECK(db.load(tempValidFile));
    }

    TEST_FIXTURE(DatabaseFixture, FindExistingUsers) {
        UserDatabase db;
        db.load(tempValidFile);

        CHECK(db.hasUser("user1"));
        CHECK_EQUAL("pass123", db.getPassword("user1"));

        CHECK(db.hasUser("user2"));
        CHECK_EQUAL("secret456", db.getPassword("user2"));

        CHECK(db.hasUser("admin"));
        CHECK_EQUAL("superadmin", db.getPassword("admin"));
    }

    TEST_FIXTURE(DatabaseFixture, NonExistentUserReturnsFalseAndThrows) {
        UserDatabase db;
        db.load(tempValidFile);

        CHECK(!db.hasUser("unknown_user"));
        CHECK_THROW(db.getPassword("unknown_user"), std::runtime_error);
    }

    TEST_FIXTURE(DatabaseFixture, EmptyDatabaseHasNoUsers) {
        UserDatabase db;
        CHECK(db.load(tempEmptyFile));
        CHECK(!db.hasUser("user1"));
        CHECK_THROW(db.getPassword("user1"), std::runtime_error);
    }

    TEST_FIXTURE(DatabaseFixture, ExpandTildePath) {
        const char* home = std::getenv("HOME");
        if (home) {
            UserDatabase db("~/test.conf");
            std::string expected = std::string(home) + "/test.conf";
            CHECK_EQUAL(expected, db.getDbPath());
        }
    }
}

