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
#include <jwt-cpp/jwt.h>
#include <memory>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

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
bool validateToken(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter &response)
{
    try
    {
        auto authHeader = request.headers().get<Pistache::Http::Header::Authorization>();
        if (!authHeader)
        {
            response.send(Pistache::Http::Code::Unauthorized, "Authorization header is missing");
            return false;
        }

        std::string token = authHeader->value();
        if (token.find("Bearer ") != 0)
        {
            response.send(Pistache::Http::Code::Unauthorized, "Invalid token format");
            return false;
        }
        token = token.substr(7); // Remove "Bearer " prefix

        if (!verifyToken(token))
        {
            response.send(Pistache::Http::Code::Unauthorized, "Invalid or expired token");
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
auto withAuthentication(Pistache::Rest::Route::Handler func) -> Pistache::Rest::Route::Handler
{
    return [func](Pistache::Rest::Request request,
                  Pistache::Http::ResponseWriter response) -> Pistache::Rest::Route::Result {
        if (!validateToken(request, response))
        {
            response.send(Pistache::Http::Code::Unauthorized, "Invalid or missing token");
            return Pistache::Rest::Route::Result::Failure;
        }
        return func(std::move(request), std::move(response));
    };
}

} // namespace AuthMiddleware


#endif
