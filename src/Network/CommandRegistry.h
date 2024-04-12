/**
 * @file CommandRegistry.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_COMMANDREGISTRY_H
#define CCFOLIO_COMMANDREGISTRY_H

#pragma once

#include "ICommand.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

class CommandRegistry
{
public:
    using CommandFactory = std::function<std::unique_ptr<ICommand>()>;

    static CommandRegistry &getInstance()
    {
        static CommandRegistry instance;
        return instance;
    }

    void registerCommand(const std::string &commandName, CommandFactory factory)
    {
        factories[commandName] = std::move(factory);
    }

    std::unique_ptr<ICommand> getCommand(const std::string &commandName)
    {
        auto it = factories.find(commandName);
        if (it != factories.end())
        {
            return it->second();
        }
        return nullptr;
    }

private:
    std::map<std::string, CommandFactory> factories;
};

#endif
