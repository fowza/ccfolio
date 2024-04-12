/**
 * @file OdbRepository.h
 * @author Frederik Pedersen
 * @brief Repository for handling CRUD operations with ODB
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ODBREPOSITORY_H
#define ODBREPOSITORY_H

#pragma once

#include "IRepository.h"
#include "LogService.h"
#include "OperationResult.h"
#include <memory>
#include <odb/database.hxx>
#include <odb/exception.hxx>
#include <odb/exceptions.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

template <typename T, typename IdType = int>
class OdbRepository : public IRepository<T, IdType>
{
    std::shared_ptr<odb::pgsql::database> postgres_db;

public:
    explicit OdbRepository(std::shared_ptr<odb::pgsql::database> postgres_db) : postgres_db(std::move(postgres_db))
    {
    }

    /**
     * @brief Create a new entity
     *
     * @param entity Entity to create
     * @return OperationResult<bool>
     */
    OperationResult<bool> Create(T entity) override
    {
        try
        {
            odb::transaction transaction(postgres_db->begin());
            postgres_db->persist(entity);
            transaction.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
    }

    /**
     * @brief Read an entity by id
     *
     * @param id Id of the entity
     * @return OperationResult<T>
     */
    OperationResult<T> Read(IdType entityId) const override
    {
        try
        {
            odb::transaction transaction(postgres_db->begin());
            std::unique_ptr<T> entity(postgres_db->load<T>(entityId));
            transaction.commit();
            return OperationResult<T>::SuccessResult(*entity);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<T>::FailureResult("Something happened, please try again later.");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<T>::FailureResult("Something happened, please try again later.");
        }
    }

    /**
     * @brief Update an entity
     *
     * @param entity Entity to update
     * @return OperationResult<bool>
     */
    OperationResult<bool> Update(const T &entity) override
    {
        try
        {
            odb::transaction transaction(postgres_db->begin());
            postgres_db->update(entity);
            transaction.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
    }

    /**
     * @brief Delete an entity by id
     *
     * @param id Id of the entity
     * @return OperationResult<bool>
     */
    OperationResult<bool> Delete(IdType entityId) override
    {
        try
        {
            odb::transaction transaction(postgres_db->begin());
            postgres_db->erase<T>(entityId);
            transaction.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult("Something happened, please try again later.");
        }
    }

    /**
     * @brief Read all entities
     *
     * @return OperationResult<std::vector<T>>
     */
    OperationResult<std::vector<T>> ReadAll() const override
    {
        try
        {
            odb::transaction transaction(postgres_db->begin());
            odb::result<T> result = postgres_db->query<T>();
            std::vector<T> entities;
            for (const auto &entity : result)
            {
                entities.push_back(entity);
            }
            transaction.commit();
            return OperationResult<std::vector<T>>::SuccessResult(entities);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<std::vector<T>>::FailureResult("Something happened, please try again later.");
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<std::vector<T>>::FailureResult("Something happened, please try again later.");
        }
    }

    [[nodiscard]] std::shared_ptr<odb::pgsql::database> database() const
    {
        return postgres_db;
    }
};

#endif
