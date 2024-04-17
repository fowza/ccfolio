#ifndef IAPIKEY_REPOSITORY_H
#define IAPIKEY_REPOSITORY_H

#include "ApiKey.h"
#include "IRepository.h"

template <typename IdType = int>
class IApiKeyRepository : public IRepository<ApiKey, IdType>
{
public:
    virtual OperationResult<bool> Disable(IdType apiKeyId) = 0;
    virtual OperationResult<std::vector<ApiKey>> ReadByUserId(IdType userId) const = 0;
};

#endif // IAPIKEY_REPOSITORY_H
