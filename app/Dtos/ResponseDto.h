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
    ResponseDto(bool isSuccess, std::optional<T> result, std::optional<std::string> errorMessage)
        : isSuccess(isSuccess), result(std::move(result)), errorMessage(std::move(errorMessage))
    {
    }

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
    [[nodiscard]] json toJson() const
    {
        json json_;
        json_["isSuccess"] = isSuccess;
        if (result.has_value())
        {
            json_["result"] = result.value().toJson();
        }
        if (errorMessage.has_value())
        {
            json_["errorMessage"] = errorMessage.value();
        }
        return json_;
    }
};

#endif // RESPONSE_DTO_H
