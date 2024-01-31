#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "IUserRepository.h"
#include <unordered_map>

class UserRepository : public IUserRepository
{
public:
    void createUser(const User &user) override
    {
        users[user.getId()] = user;
    }

    std::optional<User> getUser(int id) override
    {
        auto it = users.find(id);
        if (it != users.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

private:
    std::unordered_map<int, User> users;
};

#endif // USER_REPOSITORY_H
