//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 4
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
    os << std::hex << std::setfill('0') << std::setw(8) << i;
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

/**
 * @brief Takes a offset string and the following 5 digits, to represent bits
 *
 * @param i 32-bit uint
 * 
 * @return 0x + 5 following hex digits
 **********************************************************************************************/
std::string hex::to_hex0x20(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(5) << i;
  return std::string("0x") + os.str();
}

/**
 * @brief Takes a offset string and the following 3 digits, to represent bits
 *
 * @param i 32-bit uint
 * 
 * @return 0x + 3 following hex digits
 **********************************************************************************************/
std::string hex::to_hex0x12(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(3) << (i & 0x00000fff);
  return std::string("0x") + os.str();
}