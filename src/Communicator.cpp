#include "Communicator.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Communicator::Communicator()
    : m_serverSock(-1), m_clientSock(-1) {}

Communicator::~Communicator() {
    closeConnection();
    closeServer();
}

bool Communicator::bindAndListen(int port) {
    closeServer();

    m_serverSock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSock < 0) {
        return false;
    }

    int opt = 1;
    ::setsockopt(m_serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(static_cast<uint16_t>(port));

    if (::bind(m_serverSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        closeServer();
        return false;
    }

    if (::listen(m_serverSock, SOMAXCONN) < 0) {
        closeServer();
        return false;
    }

    return true;
}

bool Communicator::acceptConnection() {
    closeConnection();

    if (m_serverSock < 0) {
        return false;
    }

    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    m_clientSock = ::accept(m_serverSock, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
    if (m_clientSock < 0) {
        return false;
    }

    struct timeval tv{};
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    ::setsockopt(m_clientSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    ::setsockopt(m_clientSock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    return true;
}

std::string Communicator::receiveData() {
    if (m_clientSock < 0) {
        return "";
    }

    char buf[1024];
    std::memset(buf, 0, sizeof(buf));

    ssize_t bytesRead = ::recv(m_clientSock, buf, sizeof(buf) - 1, 0);
    if (bytesRead <= 0) {
        return "";
    }

    std::string result(buf, bytesRead);

    while (!result.empty() && (result.back() == '\r' || result.back() == '\n' || result.back() == '\0')) {
        result.pop_back();
    }

    return result;
}

bool Communicator::receiveData(void* buffer, size_t size) {
    if (m_clientSock < 0 || buffer == nullptr) {
        return false;
    }

    uint8_t* ptr = static_cast<uint8_t*>(buffer);
    size_t totalReceived = 0;

    while (totalReceived < size) {
        ssize_t res = ::recv(m_clientSock, ptr + totalReceived, size - totalReceived, 0);
        if (res <= 0) {
            return false;
        }
        totalReceived += static_cast<size_t>(res);
    }

    return true;
}

bool Communicator::receiveUint32(uint32_t& value) {
    return receiveData(&value, sizeof(value));
}

bool Communicator::receiveVector(std::vector<int32_t>& vec, uint32_t size) {
    vec.resize(size);
    if (size == 0) {
        return true;
    }
    return receiveData(vec.data(), size * sizeof(int32_t));
}

bool Communicator::sendData(const std::string& str) {
    return sendData(str.data(), str.size());
}

bool Communicator::sendData(int32_t result) {
    return sendData(&result, sizeof(result));
}

bool Communicator::sendData(const void* buffer, size_t size) {
    if (m_clientSock < 0 || buffer == nullptr) {
        return false;
    }

    const uint8_t* ptr = static_cast<const uint8_t*>(buffer);
    size_t totalSent = 0;

    while (totalSent < size) {
        ssize_t res = ::send(m_clientSock, ptr + totalSent, size - totalSent, MSG_NOSIGNAL);
        if (res <= 0) {
            return false;
        }
        totalSent += static_cast<size_t>(res);
    }

    return true;
}

void Communicator::closeConnection() {
    if (m_clientSock >= 0) {
        ::close(m_clientSock);
        m_clientSock = -1;
    }
}

void Communicator::closeServer() {
    if (m_serverSock >= 0) {
        ::close(m_serverSock);
        m_serverSock = -1;
    }
}
