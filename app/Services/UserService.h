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

#include "APIKeyHelper.h"
#include "IUserRepository.h"
#include "LogService.h"
#include "OperationResult.h"
#include "ResponseDto.h"
#include "User.h"
#include "UserDto.h"
#include "Utility.h"
#include <JWTHelper.h>
#include <PasswordHelper.h>
#include <argon2.h>
#include <fmt/format.h>
#include <jwt-cpp/jwt.h>
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class UserService
{
public:
    UserService(std::shared_ptr<IUserRepository> userRepository) : userRepository(std::move(userRepository))
    {
    }

    /**
     * @brief Create a new user
     *
     * @param request JSON payload with user data
     * @return ResponseDto<UserDto>
     */
    ResponseDto<UserDto> CreateUser(const std::string &request)
    {
        try
        {
            auto jsonPayload = json::parse(request);
            std::string username = jsonPayload["username"];
            std::string password = jsonPayload["password"];

            auto [hashedPassword, salt] = PasswordHelper::HashPasswordWithArgon2(password);
            std::string saltStr = Utility::toHexString(salt);

            User user{username, hashedPassword, saltStr};
            auto creationResult = userRepository->createUser(user);

            if (creationResult.IsSuccess())
            {
                UserDto userDto;
                userDto.username = username;
                userDto.token = JWTHelper::CreateJWTToken(userDto.username);
                return ResponseDto<UserDto>::Success(userDto);
            }

            LOG(LogService::LogLevel::INFO, creationResult.GetErrorMessage());
            return ResponseDto<UserDto>::Failure(creationResult.GetErrorMessage());
        }
        catch (const json::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Invalid JSON format");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Something happened, please try again later");
        }
    }

    /**
     * @brief Get a user by username
     *
     * @param username
     * @return ResponseDto<UserDto>
     */
    ResponseDto<UserDto> UserLogin(const std::string &request)
    {
        try
        {
            auto jsonPayload = json::parse(request);
            const std::string username = jsonPayload["username"];
            const std::string password = jsonPayload["password"];

            auto userResult = userRepository->getUserByUsername(username);
            if (!userResult.IsSuccess() || !userResult.GetResult().has_value())
            {
                LOG(LogService::LogLevel::INFO, userResult.GetErrorMessage());
                return ResponseDto<UserDto>::Failure(userResult.GetErrorMessage());
            }

            const auto &user = userResult.GetResult().value();
            bool isLoginSuccess = PasswordHelper::VerifyUserPassword(user.getPasswordHash(), user.getSalt(), password);

            if (!isLoginSuccess)
            {
                LOG(LogService::LogLevel::INFO,
                    fmt::format("Wrong username or password for user with username: {}", user.getUsername()));
                return ResponseDto<UserDto>::Failure("Wrong username or password!");
            }

            UserDto userDto{userResult.GetResult()->getUsername()};
            userDto.token = JWTHelper::CreateJWTToken(userDto.username);
            return ResponseDto<UserDto>::Success(userDto);
        }
        catch (const json::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Invalid JSON format");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Something happened, please try again later");
        }
    }

private:
    std::shared_ptr<IUserRepository> userRepository;
};


#endif // USER_SERVICE_H
