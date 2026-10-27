#pragma once

#include <string>
#include <unordered_map>

class UserDatabase {
public:
    explicit UserDatabase(const std::string& dbPath = "~/base/vcalc.conf");

    bool load();

    bool load(const std::string& dbPath);

    std::string getPassword(const std::string& login) const;

    bool hasUser(const std::string& login) const;

    std::string getDbPath() const;

private:
    std::string m_dbPath;
    std::unordered_map<std::string, std::string> m_users;

    static std::string expandTilde(const std::string& path);
};
