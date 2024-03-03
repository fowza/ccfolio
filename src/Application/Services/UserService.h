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
#include "config.hpp"
#include <argon2.h>
#include <fmt/format.h>
#include <jwt-cpp/jwt.h>
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
                userDto.token = CreateJWTToken(userDto);
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
            userDto.token = CreateJWTToken(userDto);
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

    /**
     * @brief Create a JWT token for a user
     *
     * @param userDto The user to create the token for
     * @return std::string The JWT token
     */
    const std::string CreateJWTToken(UserDto &userDto)
    {
        try
        {
            const std::string secretKey = std::string(secret_key);
            const std::string issuer = std::string(project_name);
            auto now = std::chrono::system_clock::now();
            auto expTime = now + std::chrono::minutes(3);

            auto token = jwt::create()
                             .set_type("JWS")
                             .set_issuer(issuer)
                             .set_issued_at(now)
                             .set_expires_at(expTime)
                             .set_payload_claim("username", jwt::claim(userDto.username))
                             .sign(jwt::algorithm::hs256{secretKey});

            return std::string(token);
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            throw std::runtime_error("Error while trying to create JWT token");
        }
    }

    /**
     * @brief Generate a random salt for hashing
     *
     * @param length The length of the salt
     * @return std::vector<uint8_t> The generated salt
     */
    std::vector<uint8_t> generateRandomSalt(size_t length)
    {
        try
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
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            throw std::runtime_error("Error while trying to generate random salt");
        }
    }

    /**
     * @brief Hash a password with Argon2
     *
     * @param password The password to hash
     * @return std::pair<std::string, std::vector<uint8_t>> The hashed password and the salt
     */
    std::pair<std::string, std::vector<uint8_t>> hashPasswordWithArgon2(const std::string &password)
    {
        try
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
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            throw std::runtime_error("Error while trying to hash password with Argon2");
        }
    }

    /**
     * @brief Verify a user's password with Argon2
     *
     * @param user The user to verify the password for
     * @param password The password to verify against the user's password
     * @return bool True if the password is correct, false otherwise
     */
    bool verifyUserPassword(const User &user, const std::string &password)
    {
        try
        {
            std::string storedHash = user.getPasswordHash();
            std::vector<uint8_t> salt(Utility::fromHexString(user.getSalt()));

            return verifyPasswordWithArgon2(password, storedHash, salt);
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            throw std::runtime_error("Error while trying to verify user password");
        }
    }

    /**
     * @brief Verify a password with Argon2
     *
     * @param password The password to verify
     * @param hashHex The hash to verify against
     * @param salt The salt to use
     * @return bool True if the password is correct, false otherwise
     */
    bool verifyPasswordWithArgon2(const std::string &password,
                                  const std::string &hashHex,
                                  const std::vector<uint8_t> &salt)
    {
        try
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
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            throw std::runtime_error("Error while trying to verify password with Argon2");
        }
    }
};


#endif // USER_SERVICE_H
