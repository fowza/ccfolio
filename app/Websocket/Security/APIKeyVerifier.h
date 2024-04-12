/**
 * @file APIKeyVerifier.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CCFOLIO_APIKEYVERIFIER_H
#define CCFOLIO_APIKEYVERIFIER_H

#include "IAPIKeyVerifier.h"

class APIKeyVerifier : public IAPIKeyVerifier
{
public:
    [[nodiscard]] bool verify(const std::string &apiKey) const override
    {
        return apiKey == "secret";
    }
};

#endif //CCFOLIO_APIKEYVERIFIER_H
