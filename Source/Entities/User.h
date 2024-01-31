#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    User() = default;
    User(int id, std::string name) : id(id), name(std::move(name)){};

    int getId() const
    {
        return id;
    };

    std::string getName() const
    {
        return name;
    };

private:
    int id;
    std::string name;
};

#endif // USER_H
