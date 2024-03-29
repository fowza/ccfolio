#include "HttpSession.h"
#include "LogService.h"
#include "WebSocketSession.h"
#include "fmt/format.h"
#include <boost/config.hpp>
#include <iostream>

template <class Body, class Allocator, class Send>
void handle_request(beast::string_view doc_root,
                    http::request<Body, http::basic_fields<Allocator>> &&req,
                    Send &&send,
                    Router &router_)
{
    HttpResponse res_;

    res_.version(req.version());
    res_.keep_alive(req.keep_alive());

    if (!router_.handleRequest(req, res_))
    {
        res_.result(http::status::not_found);
        res_.set(http::field::content_type, "text/plain");
        res_.body() = "Not Found";
        res_.prepare_payload();
    }

    return send(std::move(res_));
}

struct HttpSession::send_lambda
{
    HttpSession &self_;

    explicit send_lambda(HttpSession &self) : self_(self)
    {
    }

    template <bool isRequest, class Body, class Fields>
    void operator()(http::message<isRequest, Body, Fields> &&msg) const
    {
        auto sp = boost::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

        auto self = self_.shared_from_this();
        http::async_write(self_.stream_, *sp, [self, sp](beast::error_code ec, std::size_t bytes) {
            self->on_write(ec, bytes, sp->need_eof());
        });
    }
};

HttpSession::HttpSession(tcp::socket &&socket, boost::shared_ptr<SharedState> const &state, Router &router)
    : stream_(std::move(socket)), state_(state), router_(router)
{
}

void HttpSession::run()
{
    do_read();
}

void HttpSession::fail(beast::error_code ec, char const *what)
{
    if (ec == net::error::operation_aborted)
        return;

    LOG(LogService::LogLevel::ERROR, fmt::format("{0}: {1}", what, ec.message()));
}

void HttpSession::do_read()
{
    parser_.emplace();
    parser_->body_limit(10000);
    stream_.expires_after(std::chrono::seconds(100));

    http::async_read(stream_,
                     buffer_,
                     parser_->get(),
                     beast::bind_front_handler(&HttpSession::on_read, shared_from_this()));
}

void HttpSession::on_read(beast::error_code ec, std::size_t)
{
    if (ec == http::error::end_of_stream)
    {
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    }

    if (ec)
        return fail(ec, "read");

    if (websocket::is_upgrade(parser_->get()))
    {
        boost::make_shared<WebSocketSession>(stream_.release_socket(), state_)->run(parser_->release());
        return;
    }

    handle_request(
        state_->doc_root(),
        parser_->release(),
        [this](auto &&response) {
            using response_type = typename std::decay<decltype(response)>::type;
            auto sp = boost::make_shared<response_type>(std::forward<decltype(response)>(response));

            auto self = shared_from_this();
            http::async_write(stream_, *sp, [self, sp](beast::error_code ec, std::size_t bytes) {
                self->on_write(ec, bytes, sp->need_eof());
            });
        },
        router_);
}

void HttpSession::on_write(beast::error_code ec, std::size_t, bool close)
{
    if (ec)
        return fail(ec, "write");

    if (close)
    {
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    }

    do_read();
}
