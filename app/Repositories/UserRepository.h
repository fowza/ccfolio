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
#include "OdbRepository.h"
#include "OperationResult.h"
#include "odb/transaction.hxx"
#include <argon2.h>
#include <cstring>
#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <vector>

class UserRepository : public IUserRepository
{
private:
    std::shared_ptr<OdbRepository<User>> dbConnector;

public:
    UserRepository(std::shared_ptr<OdbRepository<User>> dbConnector) : dbConnector(std::move(dbConnector))
    {
    }

    /**
     * @brief Create a new user
     *
     * @param user User to create
     * @return OperationResult<User>
     */
    OperationResult<User> createUser(User user) override
    {
        try
        {
            auto userExist = getUserByUsername(user.getUsername());

            if (userExist.IsSuccess())
            {
                return OperationResult<User>::FailureResult(
                    fmt::format("User with username: {0} already exist.", user.getUsername()));
            }

            auto userCreated = dbConnector->Create(user);
            if (!userCreated.IsSuccess())
            {
                return OperationResult<User>::FailureResult(
                    fmt::format("Failed to create user with username: {}", user.getUsername()));
            }

            return OperationResult<User>::SuccessResult(user);
        }
        catch (const odb::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<User>::FailureResult("Something happened, please try again later");
        }
    }

    /**
     * @brief Get a user by username
     *
     * @param username Username of the user
     * @return OperationResult<std::optional<User>>
     */
    OperationResult<std::optional<User>> getUserByUsername(const std::string &username) override
    {
        try
        {
            odb::core::transaction t(dbConnector->database()->begin());
            typedef odb::query<User> Query;
            typedef odb::result<User> Result;
            Result r = dbConnector->database()->query<User>(Query::username == username);

            for (auto &user : r)
            {
                t.commit();
                return OperationResult<std::optional<User>>::SuccessResult(user);
            }

            return OperationResult<std::optional<User>>::FailureResult(
                fmt::format("Failed to find user with username: {0}", username));
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<std::optional<User>>::FailureResult("Something happened, please try again later");
        }
    }
};


#endif // USER_REPOSITORY_H
