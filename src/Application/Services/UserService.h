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
#include "LogService.h"
#include "OperationResult.h"
#include "ResponseDto.h"
#include "User.h"
#include "UserDto.h"
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
            int id = jsonPayload["id"];
            std::string username = jsonPayload["username"];
            User newUser(id, username, "password123");

            auto creationResult = userRepository->createUser(newUser);

            if (creationResult.IsSuccess())
            {
                UserDto userDto{creationResult.GetResult().getUsername()};
                return ResponseDto<UserDto>::Success(userDto);
            }
            else
            {
                LOG(LogService::LogLevel::INFO, creationResult.GetErrorMessage());
                return ResponseDto<UserDto>::Failure(creationResult.GetErrorMessage());
            }
        }
        catch (const json::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Invalid JSON format");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure(e.what());
        }
    }

    /**
     * @brief Get a user by username
     *
     * @param username
     * @return ResponseDto<UserDto>
     */
    ResponseDto<UserDto> GetUserByUsername(const std::string &username)
    {
        try
        {
            auto getResult = userRepository->getUserByUsername(username);

            if (getResult.IsSuccess())
            {
                UserDto userDto{getResult.GetResult()->getUsername()};
                return ResponseDto<UserDto>::Success(userDto);
            }
            else
            {
                LOG(LogService::LogLevel::INFO, getResult.GetErrorMessage());
                return ResponseDto<UserDto>::Failure(getResult.GetErrorMessage());
            }
        }
        catch (const json::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure("Invalid JSON format");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return ResponseDto<UserDto>::Failure(e.what());
        }
    }

private:
    std::shared_ptr<IUserRepository> userRepository;
};


#endif // USER_SERVICE_H
