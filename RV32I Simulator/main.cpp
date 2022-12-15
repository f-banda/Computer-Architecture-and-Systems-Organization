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
#include "registerfile.h"
#include "cpu_single_hart.h"
#include "rv32i_hart.h"

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
using std::cerr;

 /**
  *
  * @brief Disassemble instruction in memory
  * 
  * @param mem Memory
  *
  ********************************************************************************/
static void disassemble(const memory &mem)
{
    for (u_int pc = 0; pc < mem.get_size();)
    {
        cout << hex::to_hex32(pc) << ": " << hex::to_hex32(mem.get32(pc)) << "  ";
        cout << rv32i_decode::decode(pc, mem.get32(pc)) << endl;
        pc += 4;
    }
}

/**
 * 
 * @brief Usage function, will utilize "cerr" to provide
 * an error and end the program, if pre-reqs aren't met 
 * 
 ********************************************************************************/

static void usage()
{
    cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
    cerr << "    -d show disassembly before program execution" << endl;
    cerr << "    -i show instruction printing during execution" << endl;
    cerr << "    -l maximum number of instructions to exec" << endl;
    cerr << "    -m specify memory size (default = 0x100)" << endl;
    cerr << "    -r show register printing during execution" << endl;
    cerr << "    -z show a dump of the regs & memory after simulation" << endl;
    exit(1);
}

/**
 * @brief Main function, root of the program
 * 
 * @return 0 
 *
 ********************************************************************************/

int main(int argc, char** argv)
{
    uint32_t memory_limit = 0x100;
    int64_t insn_limit = 0;

    int opt;

    bool dflag = false;
    bool zflag = false;

    bool show_registers = false;
    bool show_instructions = false;

    while ((opt = getopt(argc, argv, "m:dirzl:")) != -1)
    {
        switch (opt)
        {
            case 'd':
                dflag = true;
                break;

            case 'i':
                show_instructions = true;
                break;

            case 'l':
                insn_limit = std::stoul(optarg, nullptr, 10);
                break;

            case 'm':
                memory_limit = std::stoul(optarg, nullptr, 16);
                break;

            case 'r':
                show_registers = true;
                break;

            case 'z':
                zflag = true;
                break;

            default: usage();
        }
    }

    memory mem(memory_limit);
    cpu_single_hart cpuSim(mem);

    cpuSim.set_show_instructions(show_instructions);
    cpuSim.set_show_registers(show_registers);
    cpuSim.reset();

    if (optind >= argc)
    {
        usage();
    }

    if (!mem.load_file(argv[optind]))
    {
        usage();
    }

    if (dflag)
    {
        disassemble(mem);
    }

    cpuSim.run(insn_limit);

    if (zflag)
    {
        cpuSim.dump(); mem.dump();
    }

    return 0;
}