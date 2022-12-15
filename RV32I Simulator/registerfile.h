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

#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <sstream>

/**
 *
 * @brief The purpose of this class is to store the state of the general-purpose
 * registers of one RISC-V hart.
 *
 ********************************************************************************/

class registerfile
{
public:

	// Constructor
	registerfile();
	
	// Initialize register x0 to zero, and rest to 0xf0f0f0f0
	void reset();

	// Assign register r the given value val. If r is zero, skip
	void set(uint32_t r, int32_t val);

	// Returns the value of register r. If r is zero, return zero
	int32_t get(uint32_t r) const;

	// Return a dump of the registers
	void dump(const std::string & hdr) const;

private:

	std::vector<int32_t> registers;
};

#endif