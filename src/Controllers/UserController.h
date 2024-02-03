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

#include "CreateUserUseCase.h"
#include "GetUserUseCase.h"
#include <nlohmann/json.hpp>
#include <pistache/router.h>

using json = nlohmann::json;

class UserController
{
public:
    UserController(std::shared_ptr<CreateUserUseCase> createUserUseCase, std::shared_ptr<GetUserUseCase> getUserUseCase)
        : createUserUseCase(std::move(createUserUseCase)), getUserUseCase(std::move(getUserUseCase))
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
        Routes::Get(router, "/users/:id", Routes::bind(&UserController::handleGetUser, this));
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
            auto body = json::parse(request.body());
            int id = body["id"];
            std::string name = body["name"];

            User newUser = createUserUseCase->execute(User(id, name));
            json responseJson = {{"id", newUser.getId()}, {"name", newUser.getName()}};

            response.send(Pistache::Http::Code::Ok, responseJson.dump());
        }
        catch (const json::exception &e)
        {
            // Handle JSON parsing errors
            response.send(Pistache::Http::Code::Bad_Request, "Invalid JSON format");
        }
        catch (const std::exception &e)
        {
            // Handle other exceptions
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        }
    }

    /**
     * @brief Handle the request for getting a user
     *
     * @param request
     * @param response
     */
    void handleGetUser(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
    {
        try
        {
            int id = std::stoi(request.param(":id").as<std::string>());

            auto user = getUserUseCase->execute(id);
            if (!user)
            {
                response.send(Pistache::Http::Code::Not_Found, "User not found");
                return;
            }

            json responseJson = {{"id", user->getId()}, {"name", user->getName()}};
            response.send(Pistache::Http::Code::Ok, responseJson.dump());
        }
        catch (const std::invalid_argument &e)
        {
            // Handle invalid argument errors
            response.send(Pistache::Http::Code::Bad_Request, "Invalid user ID");
        }
        catch (const std::exception &e)
        {
            // Handle other exceptions
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        }
    }

    std::shared_ptr<CreateUserUseCase> createUserUseCase;
    std::shared_ptr<GetUserUseCase> getUserUseCase;
    Pistache::Rest::Router router;
};

#endif // USER_CONTROLLER_H
