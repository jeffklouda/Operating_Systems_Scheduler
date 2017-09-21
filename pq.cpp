/* pq.cpp
 * Authors: Matthew D'Alonzo and Jeff Klouda
 * Multi-core process queue
 */

#include <iostream>

//  Global Variables
int nCPUs = 1;
enum Policy {fifo, rdrn, mlfq};
Policy POLICY = fifo;
unsigned int schedTime;         //  time between scheduling

using namespace std;

//  Usage Function
void usage() {
    cout <<     "Usage: ./pq [options]                                      \n"
                "                                                           \n"
                "General Options:                                           \n"
                "    -h                 Print this help message             \n"
                "    -f PATH            Path to IPC channel                 \n"
                "                                                           \n"
                "Client Options:                                            \n"
                "    add COMMAND        Add COMMAND to queue                \n"
                "    status             Query status of queue               \n"
                "    running            Query running jobs                  \n"
                "    waiting            Query waiting jobs                  \n"
                "    flush              Remove all jobs from queue          \n"
                "                                                           \n"
                "Server Options:                                            \n"
                "    -n NCPUS           Number of CPUs                      \n"
                "    -p POLICY          Scheduling policy (fifo, rdrn, mlfq)\n"
                "    -t MICROSECONDS    Time between scheduling             \n";
}
