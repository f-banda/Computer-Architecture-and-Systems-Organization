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
#include "rv32i_decode.h"

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
 * @brief Function to dissasemble the instruction stored in mem
 * 
 * @param mem 
 * 
 */
static void disassemble(const memory &mem)
{
	uint32_t memoryAddress = 0;			// Memory address
  	uint32_t insn = 0;					// Instruction
  	std::string renderedInsn;			// Decoded instruction

  	//for each byte in memory (divide by 4 because each insn is 4 bytes()
  	for (unsigned i = 0; i < mem.get_size() / 4; i++)
  	{
    	insn = mem.get32(memoryAddress);		 		//set the instruction to the one at the memory address
    	renderedInsn = rv32i_decode::decode(memoryAddress, insn);	//decode the instrution located at the memory addres

    	//output the memory address, instruction hex value, and the instruction mnemonic
    	std::cout << hex::to_hex32(memoryAddress) << ": " << hex::to_hex32(insn) << "  " << 
	    renderedInsn << std::endl;

    	memoryAddress += 4;	//increment the memory address to the next instruction
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
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	exit(1);
}

/**
 * @brief Main function, root of the program
 * 
 * @return 0 
 *
 ********************************************************************************/

int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100; // default memory size = 256 bytes
	int opt;
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch (opt)
		{
			case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;
			default: /* ’?’ */
			usage();
		}
	}

	if (optind >= argc)
		usage(); // missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	disassemble(mem);
	mem.dump();

	return 0;
}