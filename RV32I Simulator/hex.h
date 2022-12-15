//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 5
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#ifndef HEX_H
#define HEX_H

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <stdint.h>
#include <sstream>


/**
 * 
 * @brief Definition of functions
 * 
 ********************************************************************************/

class hex
{
    public :
    static std::string to_hex8 ( uint8_t i );
    static std::string to_hex32 ( uint32_t i );
    static std::string to_hex0x32 ( uint32_t i );
    static std::string to_hex0x20(uint32_t i);
    static std::string to_hex0x12(uint32_t i);
};

#endif