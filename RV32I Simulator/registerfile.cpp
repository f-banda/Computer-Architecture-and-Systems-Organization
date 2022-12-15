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

#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "registerfile.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <sstream>
#include <getopt.h>

using std::cout;
using std::endl;

/**
 *
 * @brief Constructor that calls reset() method
 *
 ********************************************************************************/
registerfile::registerfile()
{
    reset();
}

/**
 *
 * @brief Initialize register x0 to zero, rest to 0xf0f0f0f0
 *
 ********************************************************************************/
void registerfile::reset()
{
    // Initalize registers to 0xf0f0f0f0
    registers = std::vector<int32_t>(32, 0xf0f0f0f0);

    // Initialize register x0 to zero
    registers[0] = 0;
}

/**
 *
 * @brief Assign val to r, if r is zero then skip
 * 
 * @param r   Register
 * 
 * @param val Value
 *
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    // Check if r is greater than zero, if so - assign val
    if (r > 0 && r <= 32)
    {
        registers[r] = val;
    }
}

/**
 *
 * @brief Return the value of register r, if r is zero then return zero
 *
 * @param r Register
 *
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    // Check if r is greater than zero, and less than 33 - if so, return val of reg
    if (r > 0 && r <= 32)
    {
        return registers[r];
    }
    else
    {
        return 0;
    }
}

/**
 *
 * @brief Dump registers
 *
 * @param hdr String that is printed at the beginning of output lines
 *
 ********************************************************************************/
void registerfile::dump(const std::string& hdr) const
{
    cout << hdr << " x0" << std::right;

    int32_t i;

    for (i = 0; i <= 7; i++)
    {
        cout << " " << hex::to_hex32(registers.at(i));

        if (i == 3)
        {
            cout << " ";
        }
    }
    cout << endl;

    cout << hdr << " x8" << std::right;

    for (i = 8; i <= 15; i++)
    {
        cout << " " << hex::to_hex32(registers.at(i));

        if (i == 11)
        {
            cout << " ";
        }
    }
    cout << endl;

    cout << hdr << "x16" << std::right;

    for (i = 16; i <= 23; i++)
    {
        cout << " " << hex::to_hex32(registers.at(i));

        if (i == 19)
        {
            cout << " ";
        }
    }
    cout << endl;

    cout << hdr << "x24" << std::right;

    for (i = 24; i <= 31; i++)
    {
        cout << " " << hex::to_hex32(registers.at(i));

        if (i == 27)
        {
            cout << " ";
        }
    }

    cout << endl;
}