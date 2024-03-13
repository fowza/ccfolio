/**
 * @file TestController.h
 * @author Frederik Pedersen
 * @brief Controller made for testing purposes during development.
 * @version 0.1
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TEST_CONTROLLER_H
#define TEST_CONTROLLER_H

#include "AuthMiddleware.h"
#include "LogService.h"
#include <Router.h>
#include <fmt/format.h>
#include <functional>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class TestController
{
public:
    TestController(Router *router)
    {
        router->addRoute("POST",
                         "/test",
                         AuthMiddleware::withAuthentication(
                             [this](const HttpRequest &req, HttpResponse &res) { this->handleHelloWorld(req, res); }));
    }

private:
    /**
     * @brief Handle the request for creating a new user
     *
     * @param request The request
     * @param response The response
     */
    void handleHelloWorld(const HttpRequest &req, HttpResponse &res)
    {
        try
        {
            auto jsonPayload = json::parse(req.body());
            const std::string username = jsonPayload["username"];

            json j;
            j["message"] = fmt::format("Hello {}", username);

            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.body() = j.dump();
            res.prepare_payload();
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
        }
    }
};

#endif // TEST_CONTROLLER_H
