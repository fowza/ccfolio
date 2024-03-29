#include "Listener.h"
#include "HttpSession.h"
#include "LogService.h"
#include "fmt/format.h"
#include <iostream>

Listener::Listener(net::io_context &ioc,
                   tcp::endpoint endpoint,
                   boost::shared_ptr<SharedState> const &state,
                   Router &router)
    : ioc_(ioc), acceptor_(ioc), state_(state), router_(router)
{
    beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        fail(ec, "open");
        return;
    }

    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec)
    {
        fail(ec, "set_option");
        return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        fail(ec, "bind");
        return;
    }

    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec)
    {
        fail(ec, "listen");
        return;
    }
}

void Listener::run()
{
    acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
}

void Listener::fail(beast::error_code ec, char const *what)
{
    if (ec == net::error::operation_aborted)
        return;

    LOG(LogService::LogLevel::ERROR, fmt::format("{0}: {1}", what, ec.message()));
}

void Listener::on_accept(beast::error_code ec, tcp::socket socket)
{
    if (ec)
        return fail(ec, "accept");
    else
        boost::make_shared<HttpSession>(std::move(socket), state_, router_)->run();

    acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
}
