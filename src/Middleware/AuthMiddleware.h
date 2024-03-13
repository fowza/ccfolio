/**
 * @file AuthMiddleware.h
 * @author Frederik Pedersen
 * @brief Middlware for verifying jwt tokens
 * @version 0.1
 * @date 2024-03-03
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef AUTHENTICATIONMIDDLEWARE_H
#define AUTHENTICATIONMIDDLEWARE_H

#include "LogService.h"
#include "config.hpp"
#include <Router.h>
#include <fmt/format.h>
#include <jwt-cpp/jwt.h>
#include <memory>

namespace AuthMiddleware
{
/// @brief The secret key used for signing and verifying tokens
const std::string secretKey = std::string(secret_key);
const std::string issuer = std::string(project_name);

/**
     * @brief Verifies a jwt token
     *
     * @param token The token to verify
     * @return true if the token is valid, false otherwise
     */
bool verifyToken(const std::string &token)
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
bool validateToken(const HttpRequest &req, HttpResponse &res)
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

        if (!verifyToken(token))
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


/**
     * @brief Middleware for verifying jwt tokens
     *
     * @param func The function to call if the token is valid
     * @return A handler that calls the given function if the token is valid
     */
auto withAuthentication(Handler func) -> Handler
{
    return [func](const HttpRequest &req, HttpResponse &res) {
        if (!validateToken(req, res))
        {
            res.result(http::status::unauthorized);
            res.set(http::field::content_type, "application/json");
            res.body() = "Invalid or missing token";
            res.prepare_payload();
            return;
        }
        func(req, res);
    };
}


} // namespace AuthMiddleware


#endif
