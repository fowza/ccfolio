/**
 * @file TestCommand.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_TESTCOMMAND_H
#define CCFOLIO_TESTCOMMAND_H

#pragma once

#include "ICommand.h"
#include <iostream>

class TestCommand : public ICommand
{
public:
    void execute(const nlohmann::json &args) override
    {
        std::cout << "Executing command with args: " << args.dump() << "\n";
    }
};

#endif
