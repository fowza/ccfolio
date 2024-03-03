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
#include <fmt/format.h>
#include <functional>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <pistache/router.h>

using json = nlohmann::json;

class TestController
{
public:
    TestController(Pistache::Rest::Router &router) : router(router)
    {
        setupRoutes();
    }

    /**
     * @brief Set up the routes for the user controller
     *
     */
    void setupRoutes()
    {
        using namespace Pistache::Rest;

        Routes::Post(router,
                     "/test",
                     AuthMiddleware::withAuthentication(Routes::bind(&TestController::handleHelloWorld, this)));
    }

private:
    /**
     * @brief Handle the request for creating a new user
     *
     * @param request The request
     * @param response The response
     */
    void handleHelloWorld(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
    {
        try
        {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));

            auto jsonPayload = json::parse(request.body());
            const std::string username = jsonPayload["username"];

            json j;
            j["message"] = fmt::format("Hello {}", username);

            response.send(Pistache::Http::Code::Ok, j.dump());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
        }
    }

    Pistache::Rest::Router &router;
};

#endif // TEST_CONTROLLER_H
