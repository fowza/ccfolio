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
#include "Router.h"
#include "UserService.h"
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class UserController
{
public:
    UserController(std::shared_ptr<UserService> userService, Router *router) : userService(std::move(userService))
    {
        router->addRoute("POST", "/user/create", [this](const HttpRequest &req, HttpResponse &res) {
            this->handleCreateUser(req, res);
        });
        router->addRoute("POST", "/user/login", [this](const HttpRequest &req, HttpResponse &res) {
            this->handleUserLogin(req, res);
        });
    }

private:
    /**
     * @brief Handle the request for creating a new user
     *
     * @param request The request
     * @param response The response
     */
    void handleCreateUser(const HttpRequest &req, HttpResponse &res)
    {
        try
        {
            auto createUserResult = userService->CreateUser(req.body());
            auto jsonResponse = createUserResult.toJson();
            if (createUserResult.isSuccess)
            {
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = jsonResponse.dump();
                res.prepare_payload();
            }
            else
            {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = jsonResponse.dump();
                res.prepare_payload();
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
    void handleUserLogin(const HttpRequest &req, HttpResponse &res)
    {
        try
        {
            auto getUserResult = userService->UserLogin(req.body());
            auto jsonResponse = getUserResult.toJson();
            if (getUserResult.isSuccess)
            {
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = jsonResponse.dump();
                res.prepare_payload();
            }
            else
            {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = jsonResponse.dump();
                res.prepare_payload();
            }
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
        }
    }

    std::shared_ptr<UserService> userService;
};

#endif // USER_CONTROLLER_H
