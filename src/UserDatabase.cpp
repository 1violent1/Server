#include "UserDatabase.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

UserDatabase::UserDatabase(const std::string& dbPath)
    : m_dbPath(expandTilde(dbPath)) {}

std::string UserDatabase::expandTilde(const std::string& path) {
    if (path.empty() || path[0] != '~') {
        return path;
    }
    const char* home = std::getenv("HOME");
    if (!home) {
        return path;
    }
    return std::string(home) + path.substr(1);
}

std::string UserDatabase::getDbPath() const {
    return m_dbPath;
}

bool UserDatabase::load() {
    return load(m_dbPath);
}

bool UserDatabase::load(const std::string& dbPath) {
    m_dbPath = expandTilde(dbPath);
    m_users.clear();

    std::ifstream file(m_dbPath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Пропуск пустых строк и комментариев
        }

        auto delimPos = line.find(':');
        if (delimPos != std::string::npos) {
            std::string login = line.substr(0, delimPos);
            std::string password = line.substr(delimPos + 1);

            if (!password.empty() && password.back() == '\r') {
                password.pop_back();
            }

            if (!login.empty() && !password.empty()) {
                m_users[login] = password;
            }
        }
    }

    return true;
}

bool UserDatabase::hasUser(const std::string& login) const {
    return m_users.find(login) != m_users.end();
}

std::string UserDatabase::getPassword(const std::string& login) const {
    auto it = m_users.find(login);
    if (it == m_users.end()) {
        throw std::runtime_error("User not found: " + login);
    }
    return it->second;
}
