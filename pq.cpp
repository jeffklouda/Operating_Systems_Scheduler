/* pq.cpp
 * Authors: Matthew D'Alonzo and Jeff Klouda
 * Multi-core process queue
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//  Global Variables
int NCPUS = 1;
enum Policy {fifo, rdrn, mlfq};
Policy POLICY = fifo;
unsigned int SCHEDTIME;         //  time between scheduling
string PATH;

//  Usage Function
void usage(string ProgramName) {
    cout <<     "Usage: " << ProgramName << " [options]                     \n"
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

int main(int argc, char *argv[]) {
    
    //  Command Line Parsing
    string programName = argv[0];
    int argIndex = 1;
    string flag;
    while (argIndex < (argc)) {
        flag = argv[argIndex++];
        if (flag == "-h") {
            usage(programName);
            return 0;
        } else if (flag == "-f") {
            PATH = argv[argIndex++];
        } else if (flag == "-n") {
            NCPUS = atoi(argv[argIndex++]);
        } else if (flag == "-p") {
            string pChoice = argv[argIndex++];
            if (pChoice == "fifo") {
                POLICY = fifo;
            } else if (pChoice == "rdrn") {
                POLICY = rdrn;
            } else if (pChoice == "mlfq") {
                POLICY = mlfq;
            } else {
                //  Invalid scheduling policy
                usage(programName);
                return 1;
            }
        } else if (flag == "-t") {
            SCHEDTIME = atoi(argv[argIndex++]);
        } else if (flag == "add") {
            //  Run client
        } else if ( flag == "status" || flag == "running" ||
                    flag == "wating" || flag == "flush" ) {
            //  Run client
        } else {
            usage(programName);
            return 1;
        }
    }
    return EXIT_SUCCESS;
}
