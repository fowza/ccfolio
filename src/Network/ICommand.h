/**
 * @file ICommand.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_ICOMMAND_H
#define CCFOLIO_ICOMMAND_H

#pragma once

#include <memory>
#include <nlohmann/json.hpp>

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void execute(const nlohmann::json &args) = 0;
};

#endif
