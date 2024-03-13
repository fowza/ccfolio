//
// Created by fred on 3/13/24.
// Accepts incoming connections and launches the sessions
//

#ifndef CCFOLIO_LISTENER_H
#define CCFOLIO_LISTENER_H

#include "Session.h"

class Listener : public std::enable_shared_from_this<Listener>
{
    net::io_context &ioc_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    Router &router;

public:
    Listener(net::io_context &ioc, tcp::endpoint endpoint, Router &router)
        : ioc_(ioc), acceptor_(ioc), socket_(ioc), router(router)
    {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            throw std::runtime_error("Failed to open the acceptor");
        }

        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            throw std::runtime_error("Failed to bind the acceptor");
        }

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            throw std::runtime_error("Failed to listen on the acceptor");
        }
    }

    void start()
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_, [this](beast::error_code ec) {
            if (!ec)
            {
                std::make_shared<Session>(ioc_, std::move(socket_), router)->start();
            }
            do_accept();
        });
    }
};


#endif //CCFOLIO_LISTENER_H
