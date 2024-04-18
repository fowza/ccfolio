#ifndef APIKEY_H
#define APIKEY_H

#include <chrono>
#include <odb/core.hxx>
#include <odb/nullable.hxx>
#include <string>
#include <utility>

#pragma db object pointer(std::shared_ptr)
class ApiKey
{
public:
    ApiKey() = default;
    ApiKey(std::string hash,
           unsigned long user_id,
           std::string description,
           std::chrono::system_clock::time_point expires_at,
           std::string status = "active")
        : hash_(std::move(hash)), user_id_(user_id), description_(std::move(description)), status_(std::move(status))
    {
    }

    [[nodiscard]] const std::string &hash() const
    {
        return hash_;
    }
    [[nodiscard]] unsigned long owner() const
    {
        return user_id_;
    }
    [[nodiscard]] const std::string &description() const
    {
        return description_;
    }
    [[nodiscard]] const std::string &status() const
    {
        return status_;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long id_{};

    std::string hash_;
    std::string description_;
    std::string status_;

#pragma db not_null
    unsigned long user_id_{};
};

#endif // APIKEY_H
