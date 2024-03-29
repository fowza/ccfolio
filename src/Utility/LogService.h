/**
 * @file LogService.h
 * @author Frederik Pedersen
 * @brief Service for logging messages.
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LOGSERVICE_HPP
#define LOGSERVICE_HPP

#include "AppConfiguration.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

class LogService
{
public:
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

private:
    LogLevel currentLevel;
    static std::unique_ptr<LogService> instance;
    static std::mutex instanceMutex;
    std::ofstream logFile;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::queue<std::string> messageQueue;
    std::thread worker;
    std::atomic<bool> running;
    std::string currentLogDate;

public:
    LogService();
    ~LogService();
    LogService(const LogService &) = delete;
    LogService &operator=(const LogService &) = delete;

    static LogService &getInstance();
    void setLevel(LogLevel level);
    void log(const std::string &message,
             LogLevel level,
             const std::string &file,
             int line,
             const std::string &function);

private:
    std::string getCurrentDate();
    void openLogFile();
    void processMessages();
    std::string logLevelToString(LogLevel level);
};

static std::unique_ptr<LogService> instance;
static std::mutex instanceMutex;

/**
 * @brief Macro for logging messages with the given log level.
 * The message is logged with the file, line and function where the macro is called.
 */
#define LOG(level, message) LogService::getInstance().log(message, level, __FILE__, __LINE__, __FUNCTION__)

#endif // LOGSERVICE_HPP
