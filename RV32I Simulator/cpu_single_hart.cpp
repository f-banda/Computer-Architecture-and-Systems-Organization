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
#include "cpu_single_hart.h"

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
 * @brief This is a subclass of rv32i_hart that is used to represent a CPU with a single hart.
 *
 ********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
    // Set r2 to mem.get_size();
    regs.set(2, mem.get_size());

    // If exec_limit is 0, call tick() until is_halted
    if (exec_limit == 0)
    {
        while (is_halted() == false)
        {
            tick();
        }
    }
    else
    {
        while (is_halted() == false && get_insn_counter() != exec_limit)
        {
            tick();
        }
    }

    // If halted, get_halt_reason()
    if (is_halted() == true)
    {
        cout << "Execution terminated. Reason: " << get_halt_reason() << endl;
    }

    //print the number of instructions that have been executed by using get_insn_counter()
    cout << std::dec << get_insn_counter() << " instructions executed" << endl;
}