#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <string>

class MySQLConnector
{
public:
    MySQLConnector(const std::string &host,
                   const std::string &user,
                   const std::string &password,
                   const std::string &database);
    ~MySQLConnector();

    bool connect();
    void executeQuery(const std::string &query);

private:
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
    sql::mysql::MySQL_Driver *driver_;
    std::unique_ptr<sql::Connection> con_;
};

#endif // MYSQLCONNECTOR_H
