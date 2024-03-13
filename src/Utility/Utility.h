/**
 * @file Utility.h
 * @author Frederik Pedersen
 * @brief
 * @version 0.1
 * @date 2024-02-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef UTILITY_H
#define UTILITY_H

#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class Utility
{
public:
    Utility();
    ~Utility();

    static std::string toHexString(const std::vector<uint8_t> &bytes)
    {
        std::stringstream hexStream;
        hexStream << std::hex << std::setfill('0');
        for (auto byte : bytes)
        {
            hexStream << std::setw(2) << static_cast<int>(byte);
        }
        return hexStream.str();
    }

    static std::vector<uint8_t> fromHexString(const std::string &hex)
    {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2)
        {
            std::string byteString = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

private:
};

#endif
