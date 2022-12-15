//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 2 - Bitwise Operators & IEEE-754 Floating Point Number Decoding
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>

using std::cout;
using std::endl;


/**
 * printBinFloat will read the input, and print the expected details (IIEEE-754).
 *
 * @param x Obtaining the input (32-bit hex)
 * 
 *
 * @return Based off the input, return will contain hex, binary, sign, exponent,
 *         and the significant bit.
 ********************************************************************************/

void printBinFloat(uint32_t x)
{

    uint32_t bit = 0x80000000;                            // Bit placeholder for x
    int32_t exponent;                                     // Store exponent value (23-30)
    int32_t significand;                                  // Bit for significand
    bool sign = (x & 0x80000000);                         // Determine if positive or negative
    int temporary;                                        // Used to hold values


    // Print hex

    std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << x << " = ";

    // Loop through bits, printing binary value

    for (int i = 1; i <= 32; i++)
    {
        // Print initial bit

        cout << (x & bit ? '1': '0');

        // Separate every 4 bits

        if (i % 4 == 0 && i != 32)
        {
            cout << " ";
        }

        // (L) Shift 1

        bit = bit >> 1;
    }

    bit = x >> 31;

    // Print sign
    
    cout << endl;
    cout << "sign: " << bit << std::endl;

    // Assign exponent

    exponent = ((x & 0x7F800000) >> 23) - 127;

    // Print exponent

    cout << " exp: 0x" << std::setfill('0') << std::setw(8) << std::hex << exponent;

    // Print dec exponent

    cout << " (" << std::dec << exponent << ")" << std::endl;

    // Assign significand

    significand = (x & 0x007FFFFF); 

    // Print sig

    cout << " sig: 0x" << std::setfill('0') << std::setw(8) << std::hex << significand;
    cout << endl;



    /* Determining if positive or negative, inf below */

    if (sign == true)
    {
        cout << '-';
    }
    else
    {
        cout << '+';
    }


    // Inf or 0 below, along with float value


    if(exponent == (int32_t)0xffffff81 && significand == 0x00000000) 
    {

        cout << '0' << endl;

    }
    else if (exponent == (int32_t)0x00000080 && significand == 0x00000000)
    {

        cout << "inf" << endl;
    
    }


    /* Positive exponent */

    else if (exponent >= 0)
    {

        // Assign exponent + 23 to a temporary int

        temporary = exponent + 23;

        // Print '1'

        cout << '1';

        // (L) Shift 9 bits

        significand = (significand << 9);

        // Print '.' until temporary is reached, else break

        for (int i = 0; i < temporary; i++)
        {
            if(i == (int)exponent)
            {

                cout << ".";

            }

            cout << (significand & 0x80000000 ? '1' : '0');

            // (L) Shift 1 bit

            significand = (significand << 1);

            // Break

            if (i >= (int)exponent && i > 23) 
            {

                break;

            }
        }

        // Format

		cout <<  endl;
    
    }


    /* Negative exponent */

    else if (exponent < 0)
    {
        cout << "0.";

        significand = (significand << 9);

        for (int i = -1; i > exponent; i--)
        {
            cout << '0';
        }

        cout << '1';

        for (int i = 0; i < 23; i++)
        {
            cout << (significand & 0x80000000 ? '1' : '0');

            significand = (significand << 1);
        }

        cout << endl;

    }

}


/**
 * Main function that will read the input (x), and pass it into the printBinFloat
 * method which will perform the requested operations.
 ********************************************************************************/

int main()
{
    // Declare 32-bit unsigned int

    std::uint32_t  x;

    // Input loop

    while(std::cin >> std::hex >> x)
    {
    printBinFloat(x); // Calling routine
    }

    return 0;
}