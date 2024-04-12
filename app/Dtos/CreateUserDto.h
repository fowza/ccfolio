/**
 * @file UserDto.h
 * @author Frederik Pedersen
 * @brief Data transfer object for user data
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef CREATEUSER_DTO_H
#define CREATEUSER_DTO_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct CreateUserDto
{
    std::string username;
    std::string token;
    std::string apikey;

    /**
     * @brief Convert the user dto to a json object
     *
     * @return
     */
    [[nodiscard]] json toJson() const
    {
        json j;
        j["username"] = username;
        j["token"] = token;
        j["apikey"] = apikey;
        return j;
    }
};

#endif // CREATEUSER_DTO_H
