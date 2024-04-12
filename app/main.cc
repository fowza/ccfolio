#include "APIKeyVerifier.h"
#include "IAPIKeyVerifier.h"
#include "RegisterCommand.h"
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
    std::shared_ptr<odb::pgsql::database> postgres_db(new odb::pgsql::database(std::string(pg_user),
                                                                               std::string(pg_password),
                                                                               std::string(pg_database),
                                                                               std::string(pg_host),
                                                                               5432));

    auto serverAddress = net::ip::make_address(std::string(server_address));
    auto serverPort = static_cast<unsigned short>(std::atoi(std::string(server_port).c_str()));
    auto workerThreads = std::max<int>(1, std::atoi("4"));

    // Register websocket commands
    CommandRegistrar registrar;
    registerWebsocketCommands(registrar);

    net::io_context ioContext;

    // Create the router
    Router httpRouter;

    // Create repositories and services
    auto userRepository = std::make_shared<UserRepository>(std::make_shared<OdbRepository<User>>(postgres_db));
    auto userService = std::make_shared<UserService>(userRepository);

    // Create the controllers
    UserController userController(userService, &httpRouter);
    TestController testController(&httpRouter);

    // API Key verifier
    std::shared_ptr<IAPIKeyVerifier> verifier = std::make_shared<APIKeyVerifier>();

    boost::make_shared<Listener>(ioContext,
                                 tcp::endpoint{serverAddress, serverPort},
                                 boost::make_shared<SharedState>(std::string(doc_root)),
                                 httpRouter,
                                 verifier)
        ->run();
    std::cout << "Server listening on " << serverAddress << ":" << serverPort << "\n";

    net::signal_set signals(ioContext, SIGINT, SIGTERM);
    signals.async_wait([&ioContext](boost::system::error_code const &, int) { ioContext.stop(); });

    std::vector<std::thread> _threads;
    _threads.reserve(static_cast<size_t>(workerThreads - 1));
    for (auto i = workerThreads - 1; i > 0; --i)
    {
        _threads.emplace_back([&ioContext] { ioContext.run(); });
    }
    ioContext.run();

    for (auto &thread : _threads)
    {
        thread.join();
    }

    return EXIT_SUCCESS;
}
