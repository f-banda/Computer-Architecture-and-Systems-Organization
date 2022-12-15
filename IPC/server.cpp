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
#include <signal.h>
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
  cerr << "Usage: server [-l listener-port]" << endl;
  cerr << "    listener-port: The port number to which the server must listen" << endl;
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
        // Declare/Initialize
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
 * @brief Main function of the program, takes socket and accepts client connections for data transfer
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
    int opt;                    // Hold arguments
    int sock;                   // Socket (FD)
    int msgsock;                // Socket message (FD)
    socklen_t length;           // Server size
    char buf[1024];             // Byte transfer
    uint16_t totalBytes = 0;    // Total bytes (client)
    uint32_t totalLen = 0;      // Byte count (client)
    bool lCase = false;         // -l specified
    ssize_t len = 0;            // Byte count (read)
    struct sockaddr_in server;  // Socket address
    
    while ((opt = getopt(argc, argv, "l:")) != -1)
    {
        switch (opt)
        {
            case 'l':
            {
            	//Set to true so we don't use the default port
                lCase = true;

                //Set the listener port to what the user entered
                server.sin_port = htons(atoi(optarg));
            }
            break;
            
            default:
            {
                usage();
            }
        }
    }
    
    // Ignore
    signal(SIGPIPE, SIG_IGN);
    
    // Assign socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock < 0)
    {
        // Error
        perror("opening stream socket");

        // Quit
        exit(1);
    }
    
    // Socket specified
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    
    // -l case specified
    if (lCase == false)
    {
        // Port to listen from
        server.sin_port = 0;
    }
    
    // Sock name specified
    if (bind(sock, (sockaddr*)&server, sizeof(server)))
    {
        // Error
        perror("binding stream socket");

        // Quit
        exit(1);
    }
    
    // Initialize length
    length = sizeof(server);

    // Check
    if (getsockname(sock, (sockaddr*)&server, &length))
    {
        // Error
        perror("getting socket name");

        // Quit
        exit(1);
    }

    // Print port number of socket
    cout << "Socket has port #" << ntohs(server.sin_port) << endl;
    
    // Accept connection(s)
    listen(sock, 5);
    do
    {
        // Declare/Initialize
        struct sockaddr_in from;                                    // Socket display
        socklen_t from_len = sizeof(from);                          // Socket length
        msgsock = accept(sock, (struct sockaddr*)&from, &from_len); // Socket address
        totalLen = 0;                                               // Value reset
        totalBytes = 0;                                             // Value reset
        
        if (msgsock == -1)
        {
            perror("accept");
        }
        else
        {
            inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof(buf));

            // Print IP & Port
            cout << "Accepted connection from '" << buf << "', port " << ntohs(from.sin_port) << endl;
            
            // accept connection(s)
            do 
            {
            	// Read data
                if ((len = read(msgsock, buf, sizeof(buf) - 1)) < 0)
                {
                    perror("reading stream meesage");
                }

                // End connection when len == 0
                if (len == 0)
                {
                	cerr << "Ending connection" << endl;
                }
                else
                {
                    // Add to totalLen
                    totalLen += len;
                    
                    // Add to totalBytes
                	for (int i = 0; i < len; i++)
                	{
                	    totalBytes += (uint8_t)buf[i];
                	}

                    buf[len] = '\0';
                }
            } while (len != 0);
            
            // Formatting for output string
            std::ostringstream os;
            os << "Sum: " << totalBytes << " Len: " << totalLen << "\n";
            std::string str = os.str();
            const char *ch = str.c_str();
            
            // safe_write
            if (safe_write(msgsock, ch, str.length()) < 0)
            {
                perror("writing on stream socket");
            }
            close(msgsock);
        }
    } while (true);
}