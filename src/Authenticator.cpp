/**
 * @file Authenticator.cpp
 * @author Попков В.С.
 * @version 1.0
 * @date 2026
 * @copyright ИБСТ ПГУ
 * @brief Реализация методов класса Authenticator с использованием библиотеки Crypto++
 */

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "Authenticator.h"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cstdio>
#include <cstdint>

std::string Authenticator::generateSalt() {
    CryptoPP::AutoSeededRandomPool rng;
    uint64_t saltValue = 0;
    rng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(&saltValue), sizeof(saltValue));

    char buf[17];
    std::snprintf(buf, sizeof(buf), "%016llX", static_cast<unsigned long long>(saltValue));
    return std::string(buf);
}

std::string Authenticator::calculateMD5(const std::string& password, const std::string& salt) {
    std::string input = salt + password;
    std::string digest;

    CryptoPP::Weak::MD5 hash;
    CryptoPP::StringSource ss(
        input,
        true,
        new CryptoPP::HashFilter(
            hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest),
                true
            )
        )
    );

    return digest;
}
