/**
 * Created by fred on 3/13/24.
 * This class will handle incoming HTTP requests and route them accordingly
 */
#ifndef CCFOLIO_SESSION_H
#define CCFOLIO_SESSION_H

#include "Router.h"
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
    tcp::socket socket_;
    net::strand<net::io_context::executor_type> strand_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
    Router &router;

public:
    Session(net::io_context &ioc, tcp::socket socket, Router &router)
        : socket_(std::move(socket)), strand_(net::make_strand(ioc)), router(router)
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self = shared_from_this();
        http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t bytes_transferred) {
            if (!ec)
            {
                handle_request();
            }
        });
    }

    void handle_request()
    {
        res_.version(req_.version());
        res_.keep_alive(req_.keep_alive());

        if (!router.handleRequest(req_, res_))
        {
            res_.result(http::status::not_found);
            res_.set(http::field::content_type, "text/plain");
            res_.body() = "Not Found";
            res_.prepare_payload();
        }

        do_write();
    }

    void do_write()
    {
        auto self = shared_from_this();
        http::async_write(socket_, res_, [this, self](beast::error_code ec, std::size_t) {
            if (!ec)
            {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
            }
        });
    }
};

#endif //CCFOLIO_SESSION_H
