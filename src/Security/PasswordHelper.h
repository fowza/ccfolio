//
// Created by fred on 3/29/24.
//

#ifndef CCFOLIO_PASSWORDHELPER_H
#define CCFOLIO_PASSWORDHELPER_H

#include <string>
#include <vector>

class PasswordHelper
{
public:
    static std::vector<uint8_t> GenerateRandomSalt(size_t length);
    static std::pair<std::string, std::vector<uint8_t>> HashPasswordWithArgon2(const std::string &password);
    static bool VerifyUserPassword(const std::string &passwordHash,
                                   const std::string &salt,
                                   const std::string &password);
    static bool VerifyPasswordWithArgon2(const std::string &password,
                                         const std::string &hashHex,
                                         const std::vector<uint8_t> &salt);
};


#endif //CCFOLIO_PASSWORDHELPER_H
