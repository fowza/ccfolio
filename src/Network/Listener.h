//
// Created by fred on 3/17/24.
//

#ifndef CCFOLIO_LISTENER_H
#define CCFOLIO_LISTENER_H

#include "Beast.h"
#include "Net.h"
#include "Router.h"
#include <boost/smart_ptr.hpp>
#include <memory>
#include <string>

class SharedState;

class Listener : public boost::enable_shared_from_this<Listener>
{
    net::io_context &ioc_;
    tcp::acceptor acceptor_;
    boost::shared_ptr<SharedState> state_;
    Router &router_;

    void fail(beast::error_code ec, char const *what);
    void on_accept(beast::error_code ec, tcp::socket socket);

public:
    Listener(net::io_context &ioc, tcp::endpoint endpoint, boost::shared_ptr<SharedState> const &state, Router &router);

    void run();
};

#endif
