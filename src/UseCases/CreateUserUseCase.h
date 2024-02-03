/**
 * @file CreateUserUseCase.h
 * @author Frederik Pedersen
 * @brief Use case for creating a user
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CREATE_USER_USE_CASE_H
#define CREATE_USER_USE_CASE_H

#include "IUserRepository.h"
#include "User.h"
#include <memory>

class CreateUserUseCase
{
public:
    CreateUserUseCase(std::shared_ptr<IUserRepository> userRepository) : userRepository(std::move(userRepository)){};

    User execute(const User &user)
    {
        userRepository->createUser(user);
        return user;
    };

private:
    std::shared_ptr<IUserRepository> userRepository;
};

#endif // CREATE_USER_USE_CASE_H
