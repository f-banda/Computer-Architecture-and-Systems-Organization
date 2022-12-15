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

using std::cout;
using std::endl;
using std::cerr;

 /**
  *
  * @brief Memory vector siz initializing
  *
  ********************************************************************************/

memory::memory ( uint32_t siz)
{
    siz = (siz+15) & 0xfffffff0;

    // Allocate to 0xa5
    mem.resize(siz, 0xa5);
}

/**
 *
 * @brief Deconstructor
 *
 ********************************************************************************/

memory::~memory()
{
    mem.clear();
}

 /**
  *
  * @brief Check if address is illegal
  * 
  * @return True if address is in range. False if outside of range.
  *
  ********************************************************************************/

bool memory::check_illegal(uint32_t i) const
{
    if (i > get_size())
    {
        // Address out of range
        cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << endl;
        return true;
    }
    else
    {
        // Address in range
        return false;
    }
}

 /**
  *
  * @brief Returns size of memory
  *
  ********************************************************************************/

uint32_t memory::get_size() const
{
    return mem.size();
}

 /**
  *
  * @brief Check if address is illegal in memory
  * 
  * @return Address if valid, else 0
  *
  ********************************************************************************/

uint8_t memory::get8(uint32_t addr) const
{
    if(!check_illegal(addr))
    {
        return mem[addr];
    }
    else
    {
        return 0;
    }
}

 /**
  *
  * @brief get16(), by calling get8() twice and combining
  * 
  * @return 16 bits
  *
  ********************************************************************************/

uint16_t memory::get16(uint32_t addr) const
{
    // Declaring
    uint16_t x;

    // get8() twice
    x = get8(addr) | get8(addr + 1) << 8;

    // Initialize
    return x;
}

 /**
  *
  * @brief get32(), by calling get16() twice and combining
  *
  * @return 32 bits
  *
  ********************************************************************************/

uint32_t memory::get32(uint32_t addr) const
{
    // Declaring
    uint32_t x;

    // get16() twice
    x = get16(addr)  | get16(addr + 2) << 16;

    // Initialize
    return x;
}

 /**
  *
  * @brief get8() sign-extended
  *
  * @return x, sign-extended
  *
  ********************************************************************************/

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
  * @brief get16() sign-extended
  *
  * @return x, sign-extended
  *
  ********************************************************************************/

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
  * @brief get32() signed
  *
  * @return get32 address
  *
  ********************************************************************************/

int32_t memory::get32_sx(uint32_t addr) const
{
    return get32(addr);
}

 /**
  *
  * @brief Calls check_illegal() to see if address is valid
  *
  * @return Address
  *
  ********************************************************************************/

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
  * @brief get8() twice, assigning to val in Little-Endian Order
  *
  * @return Address into val
  *
  ********************************************************************************/

void memory::set16(uint32_t addr, uint16_t val)
{
    set8(addr + 1, val >> 8);

    // LSB
    set8(addr, val);

    // Assign to val
    mem[addr] = val;
}

 /**
  *
  * @brief get16() twice, assigning to val in Little-Endian Order
  *
  * @return Address into val
  *
  ********************************************************************************/

void memory::set32(uint32_t addr, uint32_t val)
{
    set16(addr + 1, val >> 8);
    set16(addr + 2, val >> 16);

    // Assign to val
    mem[addr] = val;
}

/**
 *
 * @brief Function to dump and format
 *
 ********************************************************************************/

void memory::dump() const
{
    // Create an array
    char ascii[17];
    ascii[16] = '\0';

    // Go through dump
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
    uint8_t i;

    std::ifstream infile(fname, std::ios::in|std::ios::binary);
    infile >> std::noskipws;

    // File not found
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