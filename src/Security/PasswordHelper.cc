//
// Created by fred on 3/29/24.
//

#include "PasswordHelper.h"
#include "LogService.h"
#include "Utility.h"
#include <argon2.h>
#include <config.hpp>
#include <fmt/format.h>
#include <jwt-cpp/jwt.h>
#include <memory>
#include <random>
#include <sstream>
#include <vector>

/**
     * @brief Generate a random salt for hashing
     *
     * @param length The length of the salt
     * @return std::vector<uint8_t> The generated salt
     */
std::vector<uint8_t> PasswordHelper::GenerateRandomSalt(size_t length)
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
std::pair<std::string, std::vector<uint8_t>> PasswordHelper::HashPasswordWithArgon2(const std::string &password)
{
    try
    {
        auto salt = GenerateRandomSalt(16);

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
bool PasswordHelper::VerifyUserPassword(const std::string &passwordHash,
                                        const std::string &saltInHex,
                                        const std::string &password)
{
    try
    {
        std::vector<uint8_t> salt(Utility::fromHexString(saltInHex));

        return VerifyPasswordWithArgon2(password, passwordHash, salt);
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
bool PasswordHelper::VerifyPasswordWithArgon2(const std::string &password,
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
