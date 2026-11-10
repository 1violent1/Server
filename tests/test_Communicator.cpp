#include <UnitTest++/UnitTest++.h>
#include "Communicator.h"

SUITE(CommunicatorTest) {
    TEST(UnconnectedSocketOperationsFailGracefully) {
        Communicator comm;
        CHECK_EQUAL("", comm.receiveData());

        CHECK(!comm.acceptConnection());
    }

    TEST(BindInvalidPortReturnsFalse) {
        Communicator comm;
        CHECK(!comm.bindAndListen(-1));
        CHECK(!comm.bindAndListen(0));
        CHECK(!comm.bindAndListen(70000));
    }

    TEST(BindValidHighPortSucceeds) {
        Communicator comm;
        bool success = comm.bindAndListen(45678);
        CHECK(success);
        comm.closeServer();
    }
}

