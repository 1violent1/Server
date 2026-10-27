#pragma once

#include <string>

class Authenticator {
public:
    Authenticator() = default;

    ~Authenticator() = default;

    std::string generateSalt();

    std::string calculateMD5(const std::string& password, const std::string& salt);
};
