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
    /**
     * @brief Construct a new Log Service object and start the worker thread.
     *
     */
    LogService() : currentLevel(LogLevel::DEBUG), running(true)
    {
        currentLogDate = getCurrentDate();
        openLogFile();
        worker = std::thread(&LogService::processMessages, this);
    }

    /**
     * @brief Destroy the Log Service object and make sure the worker thread is joined and the log file is closed.
     *
     */
    ~LogService()
    {
        running = false;
        condition.notify_one();
        if (worker.joinable())
        {
            worker.join();
        }
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    LogService(const LogService &) = delete;
    LogService &operator=(const LogService &) = delete;

    /**
     * @brief Get the Log Service object instance. If the instance does not exist, it is created.
     *
     * @return Pointer to log service instance.
     */
    static LogService &getInstance()
    {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (instance == nullptr)
        {
            instance.reset(new LogService());
        }
        return *instance;
    }

    /**
     * @brief Set the log level for the service.
     *
     * @param level The log level to set.
     */
    void setLevel(LogLevel level)
    {
        currentLevel = level;
    }

    /**
     * @brief Log a message with the given log level, file, line and function.
     *
     * @param message The message to log.
     * @param level The log level of the message.
     * @param file The file where the log message is called.
     * @param line The line number where the log message is called.
     * @param function The function where the log message is called.
     */
    void log(const std::string &message, LogLevel level, const std::string &file, int line, const std::string &function)
    {
        if (level < currentLevel)
            return;

        std::string filename = std::filesystem::path(file).filename().string();

        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X");
        std::string timestamp = ss.str();

        nlohmann::json logJson = {{"timestamp", timestamp},
                                  {"level", logLevelToString(level)},
                                  {"message", message},
                                  {"file", filename},
                                  {"line", line},
                                  {"function", function}};

        std::string logEntry = logJson.dump(4);
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            messageQueue.push(logEntry);
        }
        condition.notify_one();
    }

private:
    /**
     * @brief Get the current date as a string. Format: YYYY-MM-DD
     *
     * @return std::string The current date.
     */
    std::string getCurrentDate()
    {
        auto now = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(now);
        std::stringstream dateStream;
        dateStream << std::put_time(std::localtime(&nowTimeT), "%Y-%m-%d");
        return dateStream.str();
    }

    /**
     * @brief Open the log file for the current date. If the file is already open, it is closed first.
     */
    void openLogFile()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
        std::string logFileName = "/" + currentLogDate + "-application.log";
        std::string logFilePath = AppPathService::getAppDataPath() + logFileName;
        logFile.open(logFilePath, std::ios::out | std::ios::app);
        if (!logFile.is_open())
        {
            std::cerr << "Failed to open log file: " << logFilePath << std::endl;
        }
    }

    /**
     * @brief Process log messages from the queue and write them to the log file.
     * This function is run in a separate thread.
     * It will run until the service is stopped and the message queue is empty.
     */
    void processMessages()
    {
        std::unique_lock<std::mutex> lock(queueMutex, std::defer_lock);
        while (running || !messageQueue.empty())
        {
            lock.lock();
            condition.wait(lock, [this] { return !running || !messageQueue.empty(); });
            while (!messageQueue.empty())
            {
                std::string msg = messageQueue.front();
                messageQueue.pop();
                lock.unlock();
                if (logFile.is_open())
                {
                    logFile << msg << std::endl;
                }
                lock.lock();
            }
            lock.unlock();
        }
    }

    /**
     * @brief Convert a log level to a string.
     *
     * @param level The log level to convert.
     * @return std::string The log level as a string.
     */
    std::string logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }
};

std::unique_ptr<LogService> LogService::instance = nullptr;
std::mutex LogService::instanceMutex;

/**
 * @brief Macro for logging messages with the given log level.
 * The message is logged with the file, line and function where the macro is called.
 */
#define LOG(level, message) LogService::getInstance().log(message, level, __FILE__, __LINE__, __FUNCTION__)

#endif // LOGSERVICE_HPP
