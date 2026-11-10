#include <UnitTest++/UnitTest++.h>
#include "Authenticator.h"
#include <string>

SUITE(AuthenticatorTest) {
    TEST(SaltLengthIs16Chars) {
        Authenticator auth;
        std::string salt = auth.generateSalt();
        CHECK_EQUAL(16u, salt.length());
    }

    TEST(SaltCharactersAreHexUppercase) {
        Authenticator auth;
        std::string salt = auth.generateSalt();
        bool allHexUpper = true;
        for (char c : salt) {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
                allHexUpper = false;
                break;
            }
        }
        CHECK(allHexUpper);
    }

    TEST(ConsecutiveSaltsAreRandomAndUnique) {
        Authenticator auth;
        std::string salt1 = auth.generateSalt();
        std::string salt2 = auth.generateSalt();
        CHECK(salt1 != salt2);
    }

    TEST(CalculateMD5KnownVector1) {
        Authenticator auth;
        std::string hash = auth.calculateMD5("password123", "0123456789ABCDEF");
        CHECK_EQUAL("B3BEBD1797FB7F58E36621F30342D93B", hash);
    }

    TEST(CalculateMD5KnownVector2) {
        Authenticator auth;
        std::string hash = auth.calculateMD5("admin", "1234567890ABCDEF");
        CHECK_EQUAL("1C142AC1F557BD3370BB86F18E407103", hash);
    }

    TEST(CalculateMD5EmptyPassword) {
        Authenticator auth;
        std::string hash = auth.calculateMD5("", "0000000000000000");
        CHECK_EQUAL("1E4A1B03D1B6CD8A174A826F76E009F4", hash);
    }

    TEST(CalculateMD5LengthAndUppercase) {
        Authenticator auth;
        std::string hash = auth.calculateMD5("secret", "AABBCCDDEEFF0011");
        CHECK_EQUAL(32u, hash.length());
        bool allHexUpper = true;
        for (char c : hash) {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
                allHexUpper = false;
                break;
            }
        }
        CHECK(allHexUpper);
    }

    TEST(DifferentSaltsProduceDifferentHashes) {
        Authenticator auth;
        std::string hash1 = auth.calculateMD5("password", "1111111111111111");
        std::string hash2 = auth.calculateMD5("password", "2222222222222222");
        CHECK(hash1 != hash2);
    }
}

