#ifndef GET_USER_USE_CASE_H
#define GET_USER_USE_CASE_H

#include "User.h"
#include "IUserRepository.h"
#include <memory>
#include <optional>

class GetUserUseCase {
public:
    GetUserUseCase(std::shared_ptr<IUserRepository> userRepository)
    : userRepository(std::move(userRepository)) {};

    std::optional<User> execute(int userId) {
        return userRepository->getUser(userId);
    }

private:
    std::shared_ptr<IUserRepository> userRepository;
};

#endif // GET_USER_USE_CASE_H
