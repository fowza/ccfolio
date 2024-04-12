/**
 * @file ICommandRegistrar.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_ICOMMANDREGISTRAR_H
#define CCFOLIO_ICOMMANDREGISTRAR_H

#pragma once

#include "CommandRegistry.h"

class ICommandRegistrar
{
public:
    virtual ~ICommandRegistrar() = default;
    virtual void registerCommand(const std::string &commandName, CommandRegistry::CommandFactory factory) = 0;
};

#endif
