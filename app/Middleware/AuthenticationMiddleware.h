//
// Created by fred on 3/29/24.
//

#ifndef CCFOLIO_AUTHENTICATIONMIDDLEWARE_H
#define CCFOLIO_AUTHENTICATIONMIDDLEWARE_H

#include <JWTHelper.h>
#include <Router.h>

class AuthenticationMiddleware
{
public:
    /**
     * @brief Middleware for verifying jwt tokens
     *
     * @param func The function to call if the token is valid
     * @return A handler that calls the given function if the token is valid
     */
    static auto WithAuthentication(const Handler &func) -> Handler
    {
        return [func](const HttpRequest &req, HttpResponse &res) {
            if (!JWTHelper::ValidateToken(req, res))
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
};


#endif //CCFOLIO_AUTHENTICATIONMIDDLEWARE_H
