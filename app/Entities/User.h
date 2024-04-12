/**
 * @file User.h
 * @author Frederik Pedersen
 * @brief Entity for User
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef USER_H
#define USER_H

#include <odb/core.hxx>
#include <string>

#pragma db object
class User
{
public:
    User() = default;
    User(std::string username_, std::string passwordHash_, std::string salt_, std::string apiKeyHash_)
        : username(std::move(username_)), passwordHash(std::move(passwordHash_)), apiKeyHash(std::move(apiKeyHash_)),
          salt(std::move(salt_))
    {
    }

    [[nodiscard]] int getId() const
    {
        return id;
    }
    [[nodiscard]] std::string getUsername() const
    {
        return username;
    }
    [[nodiscard]] std::string getPasswordHash() const
    {
        return passwordHash;
    }
    [[nodiscard]] std::string getApiKeyHash() const
    {
        return apiKeyHash;
    }

    [[nodiscard]] std::string getSalt() const
    {
        return salt;
    }

private:
    friend class odb::access;

#pragma db id auto
    int id{};

    std::string username;
    std::string passwordHash;
    std::string apiKeyHash;

#pragma db index
    std::string salt;
};

#endif // USER_H
