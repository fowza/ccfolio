#include "OdbRepository.h"
#include "TestController.h"
#include "User-odb.hxx"
#include "UserController.h"
#include "UserRepository.h"
#include "UserService.h"
#include "config.hpp"
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/transaction.hxx>
#include <pistache/endpoint.h>
#include <pistache/router.h>

int main()
{
    // Create the database
    std::shared_ptr<odb::mysql::database> db(new odb::mysql::database(std::string(mysql_user),
                                                                      std::string(mysql_password),
                                                                      std::string(mysql_database),
                                                                      std::string(mysql_host),
                                                                      3306 /* MySQL port */));
    odb::transaction t(db->begin());

    // Create the repositories and services
    auto userRepository = std::make_shared<UserRepository>(std::make_shared<OdbRepository<User>>(db));
    auto userService = std::make_shared<UserService>(userRepository);

    // Create router object
    Pistache::Rest::Router router;

    // Create the controllers
    UserController userController(userService, router);
    TestController testController(router);

    // Create the server
    Pistache::Http::Endpoint server(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(7002)));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    server.init(opts);
    server.setHandler(router.handler());
    server.useSSL("../cert/server.crt", "../cert/server.key");
    server.serve();
}
