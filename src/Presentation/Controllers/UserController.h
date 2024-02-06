/**
 * @file UserController.h
 * @author Frederik Pedersen
 * @brief Controller for handling user CRUD operation requests
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "LogService.h"
#include "UserService.h"
#include <nlohmann/json.hpp>
#include <pistache/router.h>

using json = nlohmann::json;

class UserController
{
public:
    UserController(std::shared_ptr<UserService> userService) : userService(std::move(userService))
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

        Routes::Post(router, "/users", Routes::bind(&UserController::handleCreateUser, this));
        Routes::Get(router, "/users/:username", Routes::bind(&UserController::handleGetUserByUsername, this));
    }

    Pistache::Rest::Router &getRouter()
    {
        return router;
    }

private:
    /**
     * @brief Handle the request for creating a new user
     *
     * @param request The request
     * @param response The response
     */
    void handleCreateUser(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
    {
        try
        {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));

            auto createUserResult = userService->CreateUser(request.body());
            auto jsonResponse = createUserResult.toJson();
            if (createUserResult.isSuccess)
            {
                response.send(Pistache::Http::Code::Ok, jsonResponse.dump());
            }
            else
            {
                response.send(Pistache::Http::Code::Bad_Request, jsonResponse.dump());
            }
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
        }
    }

    /**
     * @brief Handle the request for getting a user
     *
     * @param request
     * @param response
     */
    void handleGetUserByUsername(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
    {
        try
        {
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));

            auto getUserResult = userService->GetUserByUsername(request.param(":username").as<std::string>());
            auto jsonResponse = getUserResult.toJson();
            if (getUserResult.isSuccess)
            {
                response.send(Pistache::Http::Code::Ok, jsonResponse.dump());
            }
            else
            {
                response.send(Pistache::Http::Code::Bad_Request, jsonResponse.dump());
            }
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
        }
    }

    std::shared_ptr<UserService> userService;
    Pistache::Rest::Router router;
};

#endif // USER_CONTROLLER_H
