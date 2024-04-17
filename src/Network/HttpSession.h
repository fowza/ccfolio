//
// Created by fred on 3/17/24.
//

#ifndef CCFOLIO_HTTPSESSION_H
#define CCFOLIO_HTTPSESSION_H

#include "Beast.h"
#include "Net.h"
#include "Router.h"
#include "SharedState.h"
#include <boost/optional.hpp>
#include <boost/smart_ptr.hpp>
#include <cstdlib>
#include <memory>

class HttpSession : public boost::enable_shared_from_this<HttpSession>
{
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    boost::shared_ptr<SharedState> state_;
    Router &router_;

    boost::optional<http::request_parser<http::string_body>> parser_;

    struct send_lambda;

    void fail(beast::error_code ec, char const *what);
    void do_read();
    void on_read(beast::error_code ec, std::size_t);
    void on_write(beast::error_code ec, std::size_t, bool close);

public:
    HttpSession(tcp::socket &&socket, boost::shared_ptr<SharedState> const &state, Router &router);

    void run();
};

#endif
