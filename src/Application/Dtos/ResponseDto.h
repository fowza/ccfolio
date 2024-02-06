/**
 * @file ResponseDto.h
 * @author Frederik Pedersen
 * @brief A generic response dto for returning data from the application layer
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef RESPONSE_DTO_H
#define RESPONSE_DTO_H

#include <nlohmann/json.hpp>
#include <optional>
#include <string>

using json = nlohmann::json;

template <typename T>
class ResponseDto
{
public:
    bool isSuccess;
    std::optional<T> result;
    std::optional<std::string> errorMessage;

    static ResponseDto Success(const T &data)
    {
        return {true, data, std::nullopt};
    }

    static ResponseDto Failure(const std::string &message)
    {
        return {false, std::nullopt, message};
    }

    /**
     * @brief Convert the response to a json object
     *
     * @return json
     */
    json toJson() const
    {
        json j;
        j["isSuccess"] = isSuccess;
        if (result.has_value())
        {
            j["result"] = result.value().toJson();
        }
        if (errorMessage.has_value())
        {
            j["errorMessage"] = errorMessage.value();
        }
        return j;
    }
};

#endif // RESPONSE_DTO_H
