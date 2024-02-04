#include "MySQLConnector.h"
#include <iostream>

MySQLConnector::MySQLConnector(const std::string &host,
                               const std::string &user,
                               const std::string &password,
                               const std::string &database)
    : host_(host), user_(user), password_(password), database_(database),
      driver_(sql::mysql::get_mysql_driver_instance())
{
}

MySQLConnector::~MySQLConnector()
{
    if (con_)
    {
        con_->close();
    }
}

bool MySQLConnector::connect()
{
    try
    {
        con_ = std::unique_ptr<sql::Connection>(driver_->connect(host_, user_, password_));
        con_->setSchema(database_);
        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
}

void MySQLConnector::executeQuery(const std::string &query)
{
    try
    {
        std::unique_ptr<sql::Statement> stmt(con_->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        while (res->next())
        {
            std::cout << "Data from column first_name: " << res->getString("first_name") << std::endl;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}
