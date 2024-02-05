#include "OdbRepository.h"
#include "User-odb.hxx"
#include "UserController.h"
#include "UserRepository.h"
#include "UserService.h"
#include <odb/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/transaction.hxx>
#include <pistache/endpoint.h>

int main()
{
    // Create the database
    std::shared_ptr<odb::sqlite::database> db(
        new odb::sqlite::database("sqlite.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
    odb::transaction t(db->begin());
    odb::schema_catalog::create_schema(*db);
    t.commit();

    // Create the repositories and services
    auto userRepository = std::make_shared<UserRepository>(std::make_shared<OdbRepository<User>>(db));
    auto userService = std::make_shared<UserService>(userRepository);

    // Create the controllers
    UserController userController(userService);

    // Create the server
    Pistache::Http::Endpoint server(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(7001)));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    server.init(opts);
    server.setHandler(userController.getRouter().handler());
    server.useSSL("../cert/server.crt", "../cert/server.key");
    server.serve();
}
