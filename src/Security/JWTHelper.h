//
// Created by fred on 3/29/24.
//

#ifndef CCFOLIO_JWTHELPER_H
#define CCFOLIO_JWTHELPER_H

#include <Router.h>
#include <string>

class JWTHelper
{
public:
    static std::string CreateJWTToken(const std::string &username);
    static bool VerifyToken(const std::string &token);
    static bool ValidateToken(const HttpRequest &req, HttpResponse &res);
};


#endif //CCFOLIO_JWTHELPER_H
