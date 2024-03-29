#include "WebSocketSession.h"
#include "LogService.h"
#include "fmt/format.h"
#include <iostream>

WebSocketSession::WebSocketSession(tcp::socket &&socket, boost::shared_ptr<SharedState> const &state)
    : ws_(std::move(socket)), state_(state)
{
}

WebSocketSession::~WebSocketSession()
{
    state_->leave(this);
}

void WebSocketSession::fail(beast::error_code ec, char const *what)
{
    if (ec == net::error::operation_aborted || ec == websocket::error::closed)
        return;

    LOG(LogService::LogLevel::ERROR, fmt::format("{0}: {1}", what, ec.message()));
}

void WebSocketSession::on_accept(beast::error_code ec)
{
    if (ec)
        return fail(ec, "accept");

    state_->join(this);

    ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::on_read, shared_from_this()));
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t)
{
    if (ec)
        return fail(ec, "read");

    auto message = beast::buffers_to_string(buffer_.data());

    // TODO: Parse the message for commands

    buffer_.consume(buffer_.size());

    ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::on_read, shared_from_this()));
}

void WebSocketSession::send(boost::shared_ptr<std::string const> const &ss)
{
    net::post(ws_.get_executor(), beast::bind_front_handler(&WebSocketSession::on_send, shared_from_this(), ss));
}

void WebSocketSession::on_send(boost::shared_ptr<std::string const> const &ss)
{
    queue_.push_back(ss);

    if (queue_.size() > 1)
        return;

    ws_.async_write(net::buffer(*queue_.front()),
                    beast::bind_front_handler(&WebSocketSession::on_write, shared_from_this()));
}

void WebSocketSession::on_write(beast::error_code ec, std::size_t)
{
    if (ec)
        return fail(ec, "write");

    queue_.erase(queue_.begin());

    if (!queue_.empty())
        ws_.async_write(net::buffer(*queue_.front()),
                        beast::bind_front_handler(&WebSocketSession::on_write, shared_from_this()));
}
