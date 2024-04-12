/**
 * @file IAPIKeyVerifier.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef CCFOLIO_IAPIKEYVERIFIER_H
#define CCFOLIO_IAPIKEYVERIFIER_H

#include <string>

class IAPIKeyVerifier
{
public:
    virtual ~IAPIKeyVerifier() = default;
    [[nodiscard]] virtual bool verify(const std::string &apiKey) const = 0;
};

#endif //CCFOLIO_IAPIKEYVERIFIER_H
