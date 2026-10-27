#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

class Communicator {
public:
    Communicator();

    ~Communicator();

    bool bindAndListen(int port);

    bool acceptConnection();

    std::string receiveData();

    bool receiveData(void* buffer, size_t size);

    bool receiveUint32(uint32_t& value);

    bool receiveVector(std::vector<uint32_t>& vec, uint32_t size);

    bool sendData(const std::string& str);

    bool sendData(uint32_t result);

    bool sendData(const void* buffer, size_t size);

    void closeConnection();

    void closeServer();

private:
    int m_serverSock;
    int m_clientSock;
};
