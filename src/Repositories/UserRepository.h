/**
 * @file UserRepository.h
 * @author Frederik Pedersen
 * @brief Repository for handling user CRUD operations
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "IUserRepository.h"
#include <unordered_map>

class UserRepository : public IUserRepository
{
public:
    /**
     * @brief Create a new user
     *
     * @param user
     */
    void createUser(const User &user) override
    {
        users[user.getId()] = user;
    }

    /**
     * @brief Get a user by id
     *
     * @param id The id of the user
     * @return std::optional<User> The user if found, otherwise std::nullopt
     */
    std::optional<User> getUser(int id) override
    {
        auto it = users.find(id);
        if (it != users.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

private:
    std::unordered_map<int, User> users;
};

#endif // USER_REPOSITORY_H
