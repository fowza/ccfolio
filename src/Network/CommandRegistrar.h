/**
 * @file CommandRegistrar.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#ifndef CCFOLIO_COMMANDREGISTRAR_H
#define CCFOLIO_COMMANDREGISTRAR_H

#include "ICommandRegistrar.h"

class CommandRegistrar : public ICommandRegistrar
{
public:
    void registerCommand(const std::string &commandName, CommandRegistry::CommandFactory factory) override
    {
        CommandRegistry::getInstance().registerCommand(commandName, std::move(factory));
    }
};

#endif
