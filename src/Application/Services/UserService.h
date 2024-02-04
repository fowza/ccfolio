/**
 * @file UserService.h
 * @author Frederik Pedersen
 * @brief Service for handling user related operations
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "IUserRepository.h"
#include "User.h"
#include <memory>

class UserService
{
public:
    UserService(std::shared_ptr<IUserRepository> userRepository) : userRepository(std::move(userRepository)){};

    User CreateUser(const User &user)
    {
        userRepository->createUser(user);
        return user;
    };

    std::optional<User> GetUser(int userId)
    {
        return userRepository->getUser(userId);
    }

private:
    std::shared_ptr<IUserRepository> userRepository;
};

#endif // USER_SERVICE_H
