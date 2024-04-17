#ifndef APIKEY_REPOSITORY_H
#define APIKEY_REPOSITORY_H

#include "IApiKeyRepository.h"
#include "OperationResult.h"
#include <vector>

class ApiKeyRepository : public IApiKeyRepository<>
{
public:
    ~ApiKeyRepository() = default;
    OperationResult<bool> Create(ApiKey entity) override
    {
        return OperationResult<bool>::SuccessResult(true);
    }

    [[nodiscard]] OperationResult<ApiKey> Read(int entityId) const override
    {
        return OperationResult<ApiKey>::SuccessResult(ApiKey());
    }

    OperationResult<bool> Update(const ApiKey &entity) override
    {
        return OperationResult<bool>::SuccessResult(true);
    }

    OperationResult<bool> Delete(int entityId) override
    {
        return OperationResult<bool>::SuccessResult(true);
    }

    [[nodiscard]] OperationResult<std::vector<ApiKey>> ReadAll() const override
    {
        std::vector<ApiKey> keys;
        return OperationResult<std::vector<ApiKey>>::SuccessResult(keys);
    }

    OperationResult<bool> Disable(int apiKeyId) override
    {
        return OperationResult<bool>::SuccessResult(true);
    }

    [[nodiscard]] OperationResult<std::vector<ApiKey>> ReadByUserId(int userId) const override
    {
        std::vector<ApiKey> keys;
        return OperationResult<std::vector<ApiKey>>::SuccessResult(keys);
    }
};

#endif // APIKEY_REPOSITORY_H
