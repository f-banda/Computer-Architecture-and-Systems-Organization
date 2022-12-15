//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 3
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include "hex.h"
#include "memory.h"

/**
 * @brief to_hex8 function will return a string, which contains two digits
 * 
 * @return string 
 */

std::string hex::to_hex8(uint8_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

/**
 * @brief to_hex32 function will return a string, which contains eight digits
 * 
 * @return string 
 */

std::string hex::to_hex32(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
    return os.str();
}

/**
 * @brief to_hex0x32 function will return a string, formatted
 * 
 * @return ("0x") + string 
 */

std::string hex::to_hex0x32(uint32_t i)
{
    return std::string("0x")+to_hex32(i);
}