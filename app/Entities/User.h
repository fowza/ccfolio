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
#include <utility>

#pragma db object pointer(std::shared_ptr)
class User
{
public:
    User(std::string username, std::string passwordHash, std::string salt)
        : username_(std::move(username)), passwordHash_(std::move(passwordHash)), salt_(std::move(salt))
    {
    }

    [[nodiscard]] const std::string &username() const
    {
        return username_;
    }
    [[nodiscard]] const std::string &passwordHash() const
    {
        return passwordHash_;
    }
    [[nodiscard]] const std::string &salt() const
    {
        return salt_;
    }

private:
    friend class odb::access;
    User()
    {
    }

#pragma db id auto
    unsigned long id_;

    std::string username_;
    std::string passwordHash_;
    std::string salt_;
};

#endif // USER_H
