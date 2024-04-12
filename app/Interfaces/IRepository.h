/**
 * @file IRepository.h
 * @author Frederik Pedersen
 * @brief Interface for a generic repository
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#pragma once

#include "OperationResult.h"
#include <vector>

template <typename T, typename IdType = int>
class IRepository
{
public:
    virtual OperationResult<bool> Create(T entity) = 0;
    virtual OperationResult<T> Read(IdType entityId) const = 0;
    virtual OperationResult<bool> Update(const T &entity) = 0;
    virtual OperationResult<bool> Delete(IdType entityId) = 0;
    virtual OperationResult<std::vector<T>> ReadAll() const = 0;
};


#endif
