#ifndef IUSER_REPOSITORY_H
#define IUSER_REPOSITORY_H

#include "User.h"
#include <optional>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void createUser(const User& user) = 0;
    virtual std::optional<User> getUser(int id) = 0;
};

#endif // IUSER_REPOSITORY_H
