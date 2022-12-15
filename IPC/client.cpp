//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 7
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <sstream>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using std::cout;
using std::endl;
using std::cerr;

/**
 * 
 * @brief Usage data to describe command line usage
 * 
 */
static void usage()
{
    cerr << "Usage: client [-s server-ip] server-port" << endl;
    cerr << "    server-ip: Specify the server's IPv4 number in dotted-quad format" << endl;
    cerr << "    server-port: The server port number to which the client must connect" << endl;
    exit(1);
}

/**
 * 
 * @brief Write() and will loop through full data
 * 
 * @param fd Socket descriptor
 *
 * @param buf Buffer
 *
 * @param len Length of buffer
 *
 * @return 0: Successful, -1: Error
 * 
 */
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
    while (len > 0)
    {
        ssize_t wlen = write(fd, buf, len);

        if (wlen == -1)
        {
            // Error
            return -1;
        }
        // Reduce bytes
        len -= wlen;

        // Add data
        buf += wlen;
    }

    // Return
    return len;
}

/**
 * 
 * @brief Read from socket and print
 * 
 * @param fd Socket descriptor
 *
 * @return 0: Successful, -1: Error
 * 
 */
static int print_response(int fd)
{
    // Declare/Initialize
    char buf[1024];
    int rval = 1; // Prime the loop

    while (rval > 0)
    {
        // Read from socket, to buffer
        if ((rval = read(fd, buf, sizeof(buf) - 1)) == -1)
        {
            perror("reading stream message");

            // Error
            return -1;
        }
        else if (rval > 0)
        {
            write(fileno(stdout), buf, rval);
        }
    }

    // Success
    return 0;
}

/**
 * 
 * @brief Main function of the program, create a server socket for the client
 * 
 * @param argc Count
 *
 * @param argv Values
 *
 * @return 0: Successful, -1: Error
 * 
 */
int main(int argc, char **argv)
{
    // Declare/Initialize
    int opt;                    // Store arguments
    struct sockaddr_in server;  // Socket address
    bool sCase = false;         // -s specified
    int sock;                   // Socket

    while ((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch (opt)
        {
            case 's':
            {
            	// Default address not needed
                sCase = true;

                if (inet_pton(AF_INET, optarg, &server.sin_addr) <= 0)
                {
                    cerr << optarg << ": invalid address/format" << endl;
                    exit(2);
                }
            }
            break;

            default:
            {
                usage();
            }
        }
    }

    // Read bytes from stdin, into array
    char buf[300000];
    ssize_t len = read(fileno(stdin), buf, sizeof(buf));

    // Assign socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        // Error
        perror("opening stream socket");

        // Quit
        exit(1);
    }

    // Socket to specification
    server.sin_family = AF_INET;

    // -s not specified
    if (sCase == false)
    {
        // Default address
        if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0)
        {
            // We should never get here
            cerr << "127.0.0.1" << ": invalid addresss/format" << endl;

            // Quit
            exit(2);
        }
    }

    // Port specified
    server.sin_port = htons(atoi(argv[optind]));
    
    // Link fd to server
    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
    {
        // Error
        perror("connecting stream socket");

        // Quit
        exit(1);
    }
    if (safe_write(sock, buf, len) < 0)
    {
        // Error
        perror("writing on stream socket");
    }

    // Prevent further data
    shutdown(sock, SHUT_WR);

    // print_response with socket
    print_response(sock);

    // Close Socket
    close(sock);

    return 0;
}   