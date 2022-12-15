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

using std::cout;
using std::endl;
using std::cerr;

/**
 * 
 * @brief Allocating bytes in mem vector, initializing to 0xa5.
 * 
 */

memory::memory ( uint32_t siz)
{
    // Rounding length up, mod-16
    siz = (siz+15) & 0xfffffff0;

    // Allocating byes in mem, intialize to 0xa5
    mem.resize(siz, 0xa5);
}

/**
 *
 * @brief Clean destructor
 * 
 */

memory::~memory()
{
    mem.clear();
}

/**
 *
 * @brief Checks the address in memory
 * 
 * @return True - Address is within range
 *         False - Address is NOT within range
 * 
 */

bool memory::check_illegal(uint32_t i) const
{
    if (i > get_size())
    {
        // Return true if given address is out of range
        cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << endl;
        return true;
    }
    else
    {
        // Return false if given address is within range
        return false;
    }
}

/**
 *
 * @brief Returns the rounded up number of bytes in memory
 * 
 * @return siz
 * 
 */

uint32_t memory::get_size() const
{
    return mem.size();
}

/**
 * 
 * @brief Check if address is in memory by calling check_illegal().
 * 
 * @return Value, if address is valid. Else, zero.
 * 
 */

uint8_t memory::get8(uint32_t addr) const
{
    // Check address
    if(!check_illegal(addr))
    {
        // Return address
        return mem[addr];
    }
    else
    {
        return 0;
    }
}

/**
 * 
 * @brief Calls get8() twice, combining two bytes.
 * 
 * @return 16-bit value
 * 
 */

uint16_t memory::get16(uint32_t addr) const
{
    // Combined 16-bit value
    uint16_t x;

    // Call get8() twice, and do little-endian conversion.
    x = get8(addr) | get8(addr + 1) << 8;

    // Return 16-bit value
    return x;
}

/**
 * 
 * @brief Calls get16() twice, combining two bytes.
 *  
 * @return 32-bit value
 *  
 */

uint32_t memory::get32(uint32_t addr) const
{
    // Combined 32-bit value
    uint32_t x;

    // Call get16() twice, and do little-endian conversion.
    x = get16(addr)  | get16(addr + 2) << 16;

    // Return 32-bit value
    return x;
}

/**
 * 
 * @brief Call get8() to get the sign-extended value, as 32-bit signed int
 * 
 * @return x, sign extended on 32-bit signed int
 * 
 */

int32_t memory::get8_sx(uint32_t addr) const
{
    int32_t x = (int32_t)memory::get8(addr);

    if (x&0x00000080)
    {
        x+=0xffffff00;
    }

    return x;
}

/**
 * 
 * @brief Call get16() to get the sign-extended value of 16-bit, as a 32-bit signed int
 * 
 * @return  x, 16-bit value as a 32-bit signed int
 * 
 */

int32_t memory::get16_sx(uint32_t addr) const
{
    int32_t x = (int32_t)memory::get16(addr);

    if (x&0x00008000)
    {
        x+=0xffff0000;
    }

    return x;
}

/**
 * 
 * @brief Call get32()
 * 
 * @return x, 32-bit signed int
 * 
 */

int32_t memory::get32_sx(uint32_t addr) const
{
    // Call get32, and return the value
    return get32(addr);
}

/**
 * 
 * @brief Call check_illegal() to verify address is valid
 *  
 * @return If address is valid , else return to caller.
 * 
 */

void memory::set8(uint32_t addr, uint8_t val)
{
    // Call check_illegal(), to verify address
    if (!check_illegal(addr))
    {
        // If address is valid, pass value
        mem[addr] = val;
    }
}

/**
 * 
 * @brief Call set8() twice, storing val in little-endian order
 * 
 * @return Address
 * 
 */

void memory::set16(uint32_t addr, uint16_t val)
{
    // Call set8()
    set8(addr + 1, val >> 8);

    // LSB @ address 0 format
    set8(addr, val);

    // Pass value
    mem[addr] = val;
}

/**
 * 
 * @brief Call set16() twice, storing val in little-endian order
 * 
 * @return Address 
 * 
 */

void memory::set32(uint32_t addr, uint32_t val)
{
    // Call set16(), and also set the order
    set16(addr + 1, val >> 8);
    set16(addr + 2, val >> 16);

    // Pass value
    mem[addr] = val;
}

void memory::dump() const
{
    // Array of 17 elements, set 16 to 0
    char ascii[17];
    ascii[16] = '\0';

    // Loop through dump, formatting done internally
    for (uint32_t i = 0; i < get_size(); i++)
    {
        uint8_t ch = get8(i);
        ascii[i % 16] = isprint(ch) ? ch : '.';

        if ((i % 16) == 0)
        {
            cout << hex::to_hex32(i) << ": ";
        }

        cout << hex::to_hex8(get8(i)) << " ";

        if ((i % 16) == 7)
        {
            cout << " ";
        }

        if ((i % 16) == 15)
        {
            cout << "*" << ascii << "*" << endl;
        }
    }
}

bool memory::load_file(const std::string &fname)
{
    // Open file
    std::ifstream infile(fname, std::ios::in|std::ios::binary);

    uint8_t i;
    infile >> std::noskipws;

    // File cannot be opened
    if (!infile)
    {
        cerr << "Can't open file '" << fname << "' for reading." << endl;

        return false;
    }
    else
    {
        while (!infile.eof())
        {
            for (uint32_t addr = 0; infile >> i; ++addr)
            {
                if (check_illegal(addr))
                {
                    cerr << "Program too big." << endl;
                    infile.close();
                    return false;
                }
                else
                {
                    set8(addr, i);
                }
            }
        }
    }
    infile.close();
    return true;
}