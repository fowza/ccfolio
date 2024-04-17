#include <Listener.h>
#include <OdbRepository.h>
#include <SharedState.h>
#include <TestController.h>
#include <UserController.h>
#include <UserRepository.h>
#include <UserService.h>
#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>
#include <config.hpp>
#include <iostream>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/transaction.hxx>
#include <vector>

int main()
{
    // Create the database
    std::shared_ptr<odb::pgsql::database> db(new odb::pgsql::database(std::string(pg_user),
                                                                      std::string(pg_password),
                                                                      std::string(pg_database),
                                                                      std::string(pg_host),
                                                                      5432));

    auto serverAddress = net::ip::make_address(std::string(server_address));
    auto serverPort = static_cast<unsigned short>(std::atoi(std::string(server_port).c_str()));
    auto workerThreads = std::max<int>(1, std::atoi("4"));

    net::io_context ioContext;

    // Create the router
    Router httpRouter;

    // Create repositories and services
    auto userRepository = std::make_shared<UserRepository>(std::make_shared<OdbRepository<User>>(db));
    auto userService = std::make_shared<UserService>(userRepository);

    // Create the controllers
    UserController userController(userService, &httpRouter);
    TestController testController(&httpRouter);

    boost::make_shared<Listener>(ioContext,
                                 tcp::endpoint{serverAddress, serverPort},
                                 boost::make_shared<SharedState>(std::string(doc_root)),
                                 httpRouter)
        ->run();
    std::cout << "Server listening on " << serverAddress << ":" << serverPort << std::endl;

    net::signal_set signals(ioContext, SIGINT, SIGTERM);
    signals.async_wait([&ioContext](boost::system::error_code const &, int) { ioContext.stop(); });

    std::vector<std::thread> v;
    v.reserve(workerThreads - 1);
    for (auto i = workerThreads - 1; i > 0; --i)
        v.emplace_back([&ioContext] { ioContext.run(); });
    ioContext.run();

    for (auto &t : v)
        t.join();

    return EXIT_SUCCESS;
}
