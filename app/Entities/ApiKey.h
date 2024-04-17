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
        : hash_(std::move(hash)), user_id_(user_id), description_(std::move(description)), expires_at_(expires_at),
          status_(std::move(status)), created_at_(std::chrono::system_clock::now()),
          updated_at_(std::chrono::system_clock::now())
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
    [[nodiscard]] std::chrono::system_clock::time_point expires_at() const
    {
        return expires_at_;
    }
    [[nodiscard]] std::chrono::system_clock::time_point created_at() const
    {
        return created_at_;
    }
    [[nodiscard]] std::chrono::system_clock::time_point updated_at() const
    {
        return updated_at_;
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

#pragma db null
#pragma db type("TIMESTAMP")
    std::chrono::system_clock::time_point expires_at_;

#pragma db default("CURRENT_TIMESTAMP")
#pragma db type("TIMESTAMP")
    std::chrono::system_clock::time_point created_at_;

#pragma db default("CURRENT_TIMESTAMP")
#pragma db type("TIMESTAMP")
    std::chrono::system_clock::time_point updated_at_;
};

#endif // APIKEY_H
