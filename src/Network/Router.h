//
// Created by fred on 3/13/24.
//

#ifndef CCFOLIO_ROUTER_H
#define CCFOLIO_ROUTER_H

#include <boost/beast/http.hpp>
#include <functional>
#include <string>
#include <unordered_map>

namespace http = boost::beast::http;

using HttpRequest = http::request<http::string_body>;
using HttpResponse = http::response<http::string_body>;
using Handler = std::function<void(const HttpRequest &, HttpResponse &)>;

class Router
{
private:
    std::unordered_map<std::string, Handler> routes;

public:
    void addRoute(std::string method, std::string path, Handler handler)
    {
        routes[method + ":" + path] = handler;
    }

    bool handleRequest(const HttpRequest &req, HttpResponse &res)
    {
        std::string key = req.method_string().to_string() + ":" + std::string(req.target());
        auto it = routes.find(key);
        if (it != routes.end())
        {
            it->second(req, res);
            return true;
        }
        return false;
    }
};

#endif //CCFOLIO_ROUTER_H
