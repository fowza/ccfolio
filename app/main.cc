#include <Listener.h>
#include <OdbRepository.h>
#include <TestController.h>
#include <UserController.h>
#include <UserRepository.h>
#include <UserService.h>
#include <config.hpp>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/transaction.hxx>

int main()
{
    try
    {
        // Create the database
        std::shared_ptr<odb::mysql::database> db(new odb::mysql::database(std::string(mysql_user),
                                                                          std::string(mysql_password),
                                                                          std::string(mysql_database),
                                                                          std::string(mysql_host),
                                                                          3306 /* MySQL port */));

        auto serverAddress = net::ip::make_address("127.0.0.1");
        auto serverPort = static_cast<unsigned short>(std::atoi("7004"));
        auto workerThreads = std::max<int>(1, std::atoi("4"));

        net::io_context ioContext{workerThreads};

        // Create the router
        Router httpRouter;

        // Create repositories and services
        auto userRepository = std::make_shared<UserRepository>(std::make_shared<OdbRepository<User>>(db));
        auto userService = std::make_shared<UserService>(userRepository);

        // Create the controllers
        UserController userController(userService, &httpRouter);
        TestController testController(&httpRouter);

        auto listener = std::make_shared<Listener>(ioContext, tcp::endpoint{serverAddress, serverPort}, httpRouter);
        listener->start();
        std::cout << "Server listening on " << serverAddress << ":" << serverPort << std::endl;

        std::vector<std::thread> threadPool;
        threadPool.reserve(workerThreads - 1);
        for (auto i = workerThreads - 1; i > 0; --i)
        {
            threadPool.emplace_back([&ioContext] { ioContext.run(); });
        }

        ioContext.run();

        for (auto &thread : threadPool)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
