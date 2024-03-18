#include "SharedState.h"
#include "WebSocketSession.h"

SharedState::SharedState(std::string doc_root) : doc_root_(std::move(doc_root))
{
}

void SharedState::join(WebSocketSession *session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);
}

void SharedState::leave(WebSocketSession *session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
}

void SharedState::send(std::string message)
{
    auto const ss = boost::make_shared<std::string const>(std::move(message));

    std::vector<boost::weak_ptr<WebSocketSession>> v;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        v.reserve(sessions_.size());
        for (auto p : sessions_)
            v.emplace_back(p->weak_from_this());
    }

    for (auto const &wp : v)
        if (auto sp = wp.lock())
            sp->send(ss);
}
