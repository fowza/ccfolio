#include "CreateUserUseCase.h"
#include "GetUserUseCase.h"
#include "UserController.h"
#include "UserRepository.h"
#include <pistache/endpoint.h>

int main()
{
    auto userRepository = std::make_shared<UserRepository>();
    auto createUserUseCase = std::make_shared<CreateUserUseCase>(userRepository);
    auto getUserUseCase = std::make_shared<GetUserUseCase>(userRepository);

    UserController userController(createUserUseCase, getUserUseCase);

    Pistache::Http::Endpoint server(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(7001)));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    server.init(opts);
    server.setHandler(userController.getRouter().handler());
    server.serve();
}
