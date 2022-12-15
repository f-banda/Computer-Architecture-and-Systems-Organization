//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 6
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
#include <mutex>
#include <thread>

using std::cout;
using std::endl;
using std::cerr;

/**
 *
 * Declare/Initialize Variables
 *
 ********************************************************************************/

constexpr int rows = 1000; /// < the number of rows in the work matrix
constexpr int cols = 100; /// < the number of cols in the work matrix

std::mutex stdout_lock ; /// < for serializing access to stdout

std::mutex counter_lock ; /// < for dynamic balancing only
volatile int counter = rows ; /// < for dynamic balancing only

std :: vector < int > tcount ; /// < count of rows summed for each thread
std :: vector < uint64_t > sum ; /// < the calculated sum from each thread

int work [ rows ][ cols ]; /// < the matrix to be summed

/**
 *
 * @brief This is a function to sum the rows of the matrix using static load
 *        balancing to determine which rows will be processed by each thread
 *
 * @param tid Thread ID (from main()), determine first row
 *
 * @param num_threads Determine the next row to process
 *
 ********************************************************************************/

void sum_static(int tid, int num_threads)
{
    // Declare/Initialize Variables
    uint64_t stat_sum = 0;

    // Lock
    stdout_lock.lock();

    // Output active thread ID
    cout << "Thread " << tid << " starting" << endl;

    // Unlock
    stdout_lock.unlock();

    // Loop to sum rows of matrix
    for (int j = tid; j < rows; j += num_threads)
    {
        for (int i = 0; i < cols; i++)
        {
            stat_sum += work[j][i];
        }

        tcount[tid]++;

    }

    // Lock
    stdout_lock.lock();

    // Push
    sum.push_back(stat_sum);

    // Output thread ID and sum
    cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << stat_sum << endl;

    // Unlock
    stdout_lock.unlock();
}

/**
 *
 * @brief This is a function to sum the rows of the matrix using dynamic load
 *        balancing to determine which rows will be processed by each thread
 *
 * @param tid Thread ID (from main()), determine first row
 *
 ********************************************************************************/

void sum_dynamic(int tid)
{
    // Declare/Initialize Variables
    uint64_t stat_sum = 0;
    bool end = false;

    // Lock
    stdout_lock.lock();

    // Output active thread ID
    cout << "Thread " << tid << " starting" << endl;

    // Unlock
    stdout_lock.unlock();

    while (!end)
    {
        // Counter for future use
        int save_counter;

        // Lock
        counter_lock.lock();
        {
            if (counter > 0)
            {
                --counter;
            }
            else
            {
                end = true;
            }

            save_counter = counter;

        }

        // Unlock
        counter_lock.unlock();

        if (!end)
        {
            for (int i = 0; i < cols; i++)
            {
                stat_sum += work[save_counter][i];
            }

            tcount[tid]++;

        }
    }

    // Lock
    stdout_lock.lock();
    
    // Push
    sum.push_back(stat_sum);

    // Output thread ID and sum
    cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << stat_sum << endl;

    // Unlock
    stdout_lock.unlock();

}

/**
 *
 * @brief Explanation to command line arguments
 *
 ********************************************************************************/

static void usage()
{
    cerr << "Usage: reduce [-d] [-t num] infile" << endl;
    cerr << "    -d Use dynamic load-balancing. (By default, use static load balancing.)" << endl;
    cerr << "    -t Specifies the number of threads to use. (By default, start two threads.)" << endl;
    exit(1);
}

/**
 *
 * @brief This is the main function which runs the entire program.
 *        It will accept command-line parameters, and return expected outputs.
 *
 ********************************************************************************/

int main(int argc, char **argv)
{
    // Declare/Initialize Variables
    srand(0x1234);
    bool dArg = false;                  // Was d argument provided?
    int opt;                            // Store arguments
    int total_work = 0;                 // Total work of threads
    int num_threads = 2;                // Number of threads (default 2)
    uint64_t gross_sum = 0;             // Gross sum of threads
    std::vector<std::thread*> threads;  // Threads

    // Apply rand() function to work matrix
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            work[i][j] = rand();
        }
    }

    while ((opt = getopt(argc, argv, "dt:")) != -1)
    {
        switch (opt)
        {
            // Dynamic
            case 'd':
            {
              dArg = true;
            }

            break;

            // Thread size
            case 't':
            {
	            if (optarg)
                {
                    if (atoi(optarg) <= 8)
                    {
                        num_threads = atoi(optarg);
                    }
                    else if (atoi(optarg) > 8)
                    {
                        num_threads = std::thread::hardware_concurrency();
                    }
                    if (atoi(optarg) == 0)
                    {
                        num_threads = 2;
                    }
                }
            }

            break;
        
            default:
                usage();
        }
    }

    // Apply one element, and initialize to 0 to sum and tcount
    tcount.resize(num_threads, 0);

    // Output supported threads
    cout << std::thread::hardware_concurrency() << " concurrent threads supported." << endl;
    
    // Use dynamic load balancing
    if (dArg)
    {
        // Add threads, call sum_dynamic
        for (int i = 0; i < num_threads; i++)
        {
            threads.push_back(new std::thread(sum_dynamic, i));
        }
        
        for (int i = 0; i < num_threads; i++)
        {
            // Join
            threads.at(i)->join();

            // Delete
            delete threads.at(i);

            // Adjust total_work
            total_work += tcount.at(i);
        }
    }
    // Use static load balancing
    else
    {
        // Add threads, call sum_static
        for (int i = 0; i < num_threads; i++)
        {
            threads.push_back(new std::thread(sum_static, i, num_threads));
        }
        
        for (int i = 0; i < num_threads; i++)
        {
            // Join
            threads.at(i)->join();
    
            // Delete
            delete threads.at(i);
    
            // Adjust total_work
            total_work += tcount.at(i);
        }
    }

    // Calculate gross sum
    for (int i = 0; i < num_threads; ++i)
    {
        gross_sum += sum[i];
    }

    // Output total_work, gross_sum, then return 0;
    cout << "main() exiting, total_work=" << total_work << " gross_sum=" << gross_sum << endl;

    return 0;
}