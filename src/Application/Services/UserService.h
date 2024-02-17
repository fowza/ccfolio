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
#include "Utility.h"
#include <argon2.h>
#include <fmt/format.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <vector>

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

            auto [hashedPassword, salt] = hashPasswordWithArgon2(password);
            std::string saltStr = Utility::toHexString(salt);

            User user{username, hashedPassword, saltStr};
            auto creationResult = userRepository->createUser(user);

            if (creationResult.IsSuccess())
            {
                UserDto userDto{username};
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
            bool isLoginSuccess = verifyUserPassword(user, password);

            if (!isLoginSuccess)
            {
                LOG(LogService::LogLevel::INFO,
                    fmt::format("Wrong username or password for user with username: {}", user.getUsername()));
                return ResponseDto<UserDto>::Failure("Wrong username or password!");
            }

            UserDto userDto{userResult.GetResult()->getUsername()};
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
            return ResponseDto<UserDto>::Failure(e.what());
        }
    }

private:
    std::shared_ptr<IUserRepository> userRepository;

    std::vector<uint8_t> generateRandomSalt(size_t length)
    {
        std::vector<uint8_t> salt(length);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 255);

        for (auto &byte : salt)
        {
            byte = static_cast<uint8_t>(dist(gen));
        }

        return salt;
    }

    std::pair<std::string, std::vector<uint8_t>> hashPasswordWithArgon2(const std::string &password)
    {
        auto salt = generateRandomSalt(16);

        const uint32_t t_cost = 2;
        const uint32_t m_cost = (1 << 16);
        const uint32_t parallelism = 1;

        std::vector<uint8_t> hashRaw(32);

        int result = argon2i_hash_raw(t_cost,
                                      m_cost,
                                      parallelism,
                                      password.data(),
                                      password.size(),
                                      salt.data(),
                                      salt.size(),
                                      hashRaw.data(),
                                      hashRaw.size());
        if (result != ARGON2_OK)
        {
            throw std::runtime_error("Hashing failed with Argon2");
        }

        std::stringstream hexStream;
        for (auto byte : hashRaw)
        {
            hexStream << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
        }

        return {hexStream.str(), salt};
    }

    bool verifyUserPassword(const User &user, const std::string &password)
    {
        std::string storedHash = user.getPasswordHash();
        std::vector<uint8_t> salt(Utility::fromHexString(user.getSalt()));

        return verifyPasswordWithArgon2(password, storedHash, salt);
    }


    bool verifyPasswordWithArgon2(const std::string &password,
                                  const std::string &hashHex,
                                  const std::vector<uint8_t> &salt)
    {
        const uint32_t t_cost = 2;
        const uint32_t m_cost = (1 << 16);
        const uint32_t parallelism = 1;

        std::vector<uint8_t> hashRaw(32);

        int result = argon2i_hash_raw(t_cost,
                                      m_cost,
                                      parallelism,
                                      password.data(),
                                      password.size(),
                                      salt.data(),
                                      salt.size(),
                                      hashRaw.data(),
                                      hashRaw.size());
        if (result != ARGON2_OK)
        {
            throw std::runtime_error("Hashing failed with Argon2");
        }

        std::stringstream hexStream;
        for (auto byte : hashRaw)
        {
            hexStream << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
        }

        return hexStream.str() == hashHex;
    }
};


#endif // USER_SERVICE_H
