#include "UserController.h"
#include "UserRepository.h"
#include "UserService.h"
#include <pistache/endpoint.h>

int main()
{
    auto userRepository = std::make_shared<UserRepository>();
    auto userService = std::make_shared<UserService>(userRepository);

    UserController userController(userService);

    Pistache::Http::Endpoint server(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(7001)));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    server.init(opts);
    server.setHandler(userController.getRouter().handler());
    server.serve();
}
