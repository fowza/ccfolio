/**
 * @file IUserRepository.h
 * @author Frederik Pedersen
 * @brief Interface for the UserRepository
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef IUSER_REPOSITORY_H
#define IUSER_REPOSITORY_H

#include "OperationResult.h"
#include "User.h"
#include <optional>

class IUserRepository
{
public:
    virtual ~IUserRepository() = default;
    virtual OperationResult<std::optional<User>> getUserByUsername(const std::string &username) = 0;
    virtual OperationResult<User> createUser(User user) = 0;
};

#endif // IUSER_REPOSITORY_H
