//
// Created by fred on 3/29/24.
//

#include "JWTHelper.h"
#include <LogService.h>
#include <config.hpp>
#include <jwt-cpp/jwt.h>

/// @brief The secret key used for signing and verifying tokens
const std::string secretKey = std::string(secret_key);
const std::string issuer = std::string(project_name);

/**
     * @brief Create a JWT token for a user
     *
     * @param userDto The user to create the token for
     * @return std::string The JWT token
     */
std::string JWTHelper::CreateJWTToken(const std::string &username)
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
                         .set_payload_claim("username", jwt::claim(username))
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
     * @brief Verifies a jwt token
     *
     * @param token The token to verify
     * @return true if the token is valid, false otherwise
     */
bool JWTHelper::VerifyToken(const std::string &token)
{
    try
    {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{secretKey}).with_issuer(issuer);

        verifier.verify(decoded);
        return true;
    }
    catch (const std::exception &e)
    {
        LOG(LogService::LogLevel::ERROR, fmt::format("Error while trying to verify token. Error: {0}", e.what()));
    }
    return false;
}

/**
     * @brief Validates a jwt token from the Authorization header
     *
     * @param request The request to validate
     * @param response The response to send if the token is invalid
     * @return true if the token is valid, false otherwise
     */
bool JWTHelper::ValidateToken(const HttpRequest &req, HttpResponse &res)
{
    try
    {
        auto authHeader = req.find("Authorization");
        if (authHeader == req.end())
        {
            res.result(http::status::unauthorized);
            res.set(http::field::content_type, "application/json");
            res.body() = "Authorization header is missing";
            res.prepare_payload();
            return false;
        }

        std::string token = std::string(authHeader->value());
        if (token.find("Bearer ") != 0)
        {
            res.result(http::status::unauthorized);
            res.set(http::field::content_type, "application/json");
            res.body() = "Invalid token format";
            res.prepare_payload();
            return false;
        }
        token = token.substr(7); // Remove "Bearer " prefix

        if (!VerifyToken(token))
        {
            res.result(http::status::unauthorized);
            res.set(http::field::content_type, "application/json");
            res.body() = "Invalid or expired token";
            res.prepare_payload();
            return false;
        }

        return true; // Authentication successful
    }
    catch (const std::exception &e)
    {
        LOG(LogService::LogLevel::ERROR, fmt::format("Error while trying to validate token. Error: {0}", e.what()));
    }
    return false;
}
