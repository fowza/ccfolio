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
#ifndef USER_DTO_H
#define USER_DTO_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct UserDto
{
    std::string username;

    /**
     * @brief Convert the user dto to a json object
     *
     * @return
     */
    json toJson() const
    {
        json j;
        j["username"] = username;
        return j;
    }
};

#endif // USER_DTO_H
