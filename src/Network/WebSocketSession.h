//
// Created by fred on 3/17/24.
//

#ifndef CCFOLIO_WEBSOCKETSESSION_H
#define CCFOLIO_WEBSOCKETSESSION_H

#include "Beast.h"
#include "IAPIKeyVerifier.h"
#include "Net.h"
#include "SharedState.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

class SharedState;

class WebSocketSession : public boost::enable_shared_from_this<WebSocketSession>
{
    beast::flat_buffer buffer_;
    websocket::stream<beast::tcp_stream> ws_;
    boost::shared_ptr<SharedState> state_;
    std::vector<boost::shared_ptr<std::string const>> queue_;
    std::shared_ptr<IAPIKeyVerifier> apiKeyVerifier;

    void fail(beast::error_code ec, char const *what);
    void on_accept(beast::error_code ec);
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);

public:
    WebSocketSession(tcp::socket &&socket,
                     boost::shared_ptr<SharedState> const &state,
                     std::shared_ptr<IAPIKeyVerifier> verifier);

    ~WebSocketSession();

    template <class Body, class Allocator>
    void run(http::request<Body, http::basic_fields<Allocator>> req);

    void send(boost::shared_ptr<std::string const> const &ss);

private:
    void on_send(boost::shared_ptr<std::string const> const &ss);
};

template <class Body, class Allocator>
void WebSocketSession::run(http::request<Body, http::basic_fields<Allocator>> req)
{
    auto target = req.target();
    std::string apiKeyParam = "?api_key=";
    auto apiKeyPos = target.find(apiKeyParam);
    if (apiKeyPos == std::string::npos)
    {
        fail(beast::error_code{}, "API key not provided");
        return;
    }

    auto apiKey = target.substr(apiKeyPos + apiKeyParam.length());

    if (!apiKeyVerifier->verify(std::string(apiKey)))
    {
        fail(beast::error_code{}, "Invalid API key");
        return;
    }

    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    ws_.set_option(websocket::stream_base::decorator([](websocket::response_type &res) {
        res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket");
    }));

    ws_.async_accept(req, beast::bind_front_handler(&WebSocketSession::on_accept, shared_from_this()));
}

#endif
