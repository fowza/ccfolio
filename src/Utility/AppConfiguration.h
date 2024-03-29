/**
 * @file AppPathService.h
 * @author Frederik Pedersen
 * @brief Service for getting the application's data directory.
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APPPATHSERVICE_HPP
#define APPPATHSERVICE_HPP

#include <config.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <string>

class AppPathService
{
public:
    /**
     * @brief Get the App Data Path as a string.
     * If the path does not exist, it will be created.
     *
     * @return std::string The path to the application's data directory.
     */
    static std::string getAppDataPath()
    {
        std::string folderName = fmt::format("{0}/logs", project_name);
        auto homePath = std::filesystem::path(std::getenv("HOME")) / folderName;
        std::filesystem::create_directories(homePath);
        return homePath.string();
    }
};

#endif // APPPATHSERVICE_HPP
