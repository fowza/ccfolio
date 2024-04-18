/**
 * @file OperationResult.h
 * @author Frederik Pedersen
 * @brief Class template for operation results
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef OPERATION_RESULT_H
#define OPERATION_RESULT_H

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

/**
 * @brief generic enum for operation status
 *
 */
enum class OperationStatus
{
    Success,
    Failure
};

/**
 * @brief Generic class template for operation results
 *
 * @tparam T Type of the result
 */
template <typename T>
class OperationResult
{
public:
    /**
     * @brief Constructors for success and failure scenarios
     *
     * @param status OperationStatus
     * @param result Result of the operation
     * @param errorMessage Error message if the operation failed
     */
    explicit OperationResult(OperationStatus status, T result = T(), std::string errorMessage = "")
        : _status(status), result(std::move(result)), errorMessage(std::move(errorMessage))
    {
    }

    /**
     * @brief Factory method for creating a successful operation result
     *
     * @param result
     * @return
     */
    static OperationResult SuccessResult(T result)
    {
        return OperationResult(OperationStatus::Success, std::move(result));
    }

    /**
     * @brief Factory method for creating a failed operation result
     *
     * @param errorMessage
     * @return
     */
    static OperationResult FailureResult(std::string errorMessage)
    {
        return OperationResult(OperationStatus::Failure, T(), std::move(errorMessage));
    }

    /**
     * @brief Check if the operation was successful
     *
     * @return
     */
    bool IsSuccess() const
    {
        return _status == OperationStatus::Success;
    }

    /**
     * @brief Get the result of the operation
     *
     * @return
     */
    const T &GetResult() const
    {
        if (!IsSuccess())
        {
            throw std::logic_error("Attempted to access result of a failed operation.");
        }
        return result;
    }

    const std::string &GetErrorMessage() const
    {
        if (IsSuccess())
        {
            throw std::logic_error("Attempted to access error message of a successful operation.");
        }
        return errorMessage;
    }

private:
    OperationStatus _status;
    T result;
    std::string errorMessage;
};

#endif // OPERATION_RESULT_H
