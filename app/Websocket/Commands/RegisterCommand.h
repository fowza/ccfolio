/**
 * @file RegisterCommand.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_REGISTERCOMMAND_H
#define CCFOLIO_REGISTERCOMMAND_H

#pragma once

#include <CommandRegistrar.h>
#include <TestCommand.h>

void registerWebsocketCommands(CommandRegistrar &registrar)
{
    registrar.registerCommand("TestCommand", []() { return std::make_unique<TestCommand>(); });
}

#endif
