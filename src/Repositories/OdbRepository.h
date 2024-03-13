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
#include "User-odb.hxx"
#include <memory>
#include <odb/database.hxx>
#include <odb/exception.hxx>
#include <odb/exceptions.hxx>
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>

template <typename T, typename IdType = int>
class OdbRepository : public IRepository<T, IdType>
{
    std::shared_ptr<odb::mysql::database> db;

public:
    explicit OdbRepository(std::shared_ptr<odb::mysql::database> db) : db(std::move(db))
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
            odb::transaction t(db->begin());
            db->persist(entity);
            t.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
        }
    }

    /**
     * @brief Read an entity by id
     *
     * @param id Id of the entity
     * @return OperationResult<T>
     */
    OperationResult<T> Read(IdType id) const override
    {
        try
        {
            odb::transaction t(db->begin());
            std::unique_ptr<T> entity(db->load<T>(id));
            t.commit();
            return OperationResult<T>::SuccessResult(*entity);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<T>::FailureResult(e.what());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<T>::FailureResult(e.what());
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
            odb::transaction t(db->begin());
            db->update(entity);
            t.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
        }
    }

    /**
     * @brief Delete an entity by id
     *
     * @param id Id of the entity
     * @return OperationResult<bool>
     */
    OperationResult<bool> Delete(IdType id) override
    {
        try
        {
            odb::transaction t(db->begin());
            db->erase<T>(id);
            t.commit();
            return OperationResult<bool>::SuccessResult(true);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<bool>::FailureResult(e.what());
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
            odb::transaction t(db->begin());
            odb::result<T> result = db->query<T>();
            std::vector<T> entities;
            for (const auto &entity : result)
            {
                entities.push_back(entity);
            }
            t.commit();
            return OperationResult<std::vector<T>>::SuccessResult(entities);
        }
        catch (const odb::connection_lost &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<std::vector<T>>::FailureResult(e.what());
        }
        catch (const std::exception &e)
        {
            LOG(LogService::LogLevel::ERROR, e.what());
            return OperationResult<std::vector<T>>::FailureResult(e.what());
        }
    }

    std::shared_ptr<odb::mysql::database> database() const
    {
        return db;
    }
};

#endif
