/**
 * @file GetUserUseCase.h
 * @author Frederik Pedersen
 * @brief Use case for getting a user
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef GET_USER_USE_CASE_H
#define GET_USER_USE_CASE_H

#include "IUserRepository.h"
#include "User.h"
#include <memory>
#include <optional>

class GetUserUseCase
{
public:
    GetUserUseCase(std::shared_ptr<IUserRepository> userRepository) : userRepository(std::move(userRepository)){};

    std::optional<User> execute(int userId)
    {
        return userRepository->getUser(userId);
    }

private:
    std::shared_ptr<IUserRepository> userRepository;
};

#endif // GET_USER_USE_CASE_H
