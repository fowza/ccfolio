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
    User(int id_, std::string username_, std::string passwordHash_)
        : id(id_), username(std::move(username_)), passwordHash(std::move(passwordHash_))
    {
    }

    int getId() const
    {
        return id;
    }
    std::string getUsername() const
    {
        return username;
    }
    std::string getPasswordHash() const
    {
        return passwordHash;
    }

private:
    friend class odb::access;

#pragma db id
    int id{};

    std::string username;
    std::string passwordHash;
};

#endif // USER_H
