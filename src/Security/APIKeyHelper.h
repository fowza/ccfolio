/**
 * @file APIKeyHelper.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_APIKEYHELPER_H
#define CCFOLIO_APIKEYHELPER_H

#include <argon2.h>
#include <config.hpp>
#include <fmt/format.h>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class APIKeyHelper
{
public:
    static std::string createAPIKey(size_t length = 32)
    {
        std::vector<unsigned char> apiKey(length);
        std::random_device random;
        std::mt19937 gen(random());
        std::uniform_int_distribution<> dis(0, 255);

        for (auto &byte : apiKey)
        {
            byte = static_cast<unsigned char>(dis(gen));
        }

        std::stringstream hexStr;
        for (auto &byte : apiKey)
        {
            hexStr << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
        }

        return hexStr.str();
    }

    static std::string hashAPIKey(const std::string &apiKey, const std::vector<unsigned char> &salt)
    {
        if (salt.empty())
        {
            throw std::invalid_argument("Salt cannot be empty.");
        }

        const size_t hashLength = 32;
        //unsigned char hash[hashLength];
        std::array<unsigned char, hashLength> hash{};

        const uint32_t t_cost = 2;
        const uint32_t m_cost = (1 << 16);
        const uint32_t parallelism = 1;

        int result = argon2i_hash_raw(t_cost,
                                      m_cost,
                                      parallelism,
                                      apiKey.data(),
                                      apiKey.length(),
                                      salt.data(),
                                      salt.size(),
                                      hash.data(),
                                      hashLength);
        if (result != ARGON2_OK)
        {
            throw std::runtime_error(fmt::format("Argon2 hashing failed: {}", argon2_error_message(result)));
        }

        std::stringstream hashStr;
        for (size_t i = 0; i < hashLength; ++i)
        {
            hashStr << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(hash[i]);
        }

        return hashStr.str();
    }
};

#endif //CCFOLIO_APIKEYHELPER_H
