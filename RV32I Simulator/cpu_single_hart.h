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

#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <sstream>

#include "rv32i_hart.h"

/**
 *
 * @brief This is a subclass of rv32i_hart that is used to represent a CPU with a single hart.
 *
 ********************************************************************************/

class cpu_single_hart : public rv32i_hart
{
public:

    // Constructor to pass the memory class instance to the constructor in the base class
    cpu_single_hart(memory& mem) : rv32i_hart(mem) {}

    // Set register x2 to the memory size (mem.get_size())
    void run(uint64_t exec_limit);

};

#endif