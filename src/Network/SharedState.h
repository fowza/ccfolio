//
// Created by fred on 3/17/24.
//

#ifndef CCFOLIO_SHAREDSTATE_H
#define CCFOLIO_SHAREDSTATE_H

#include <boost/smart_ptr.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>

class WebSocketSession;

class SharedState
{
    std::string const doc_root_;
    std::mutex mutex_;
    std::unordered_set<WebSocketSession *> sessions_;

public:
    explicit SharedState(std::string doc_root);

    std::string const &doc_root() const noexcept
    {
        return doc_root_;
    }

    void join(WebSocketSession *session);
    void leave(WebSocketSession *session);
    void send(std::string message);
};

#endif
