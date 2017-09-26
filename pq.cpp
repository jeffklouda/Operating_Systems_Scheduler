/* pq.cpp
 * Main function of implementation of scheduler.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

#include "pq.h"          //header file

#include <sstream>       //stringstreams
#include <stdbool.h>     //true, false
#include <errno.h>       //errno
#include <unistd.h>      //NULL, etc
#include <signal.h>      //signals
#include <iostream>      //cin, cout
#include <string>        //std::string
#include <stdio.h>       //standard i/o
#include <stdlib.h>      //size_t, etc
#include <poll.h>        //poll()
#include <string.h>      //c_string stuff
#include <time.h>        //time(NULL)
#include <sys/types.h>   //defines types
#include <sys/wait.h>    //waitpid()

using namespace std;

//  Global Variables
time_t       prev_time       = 0;                //Used for Priority Boost (MLFQ)
int          NCPUS           = 1;                //Number of CPUs (for running mutiple processes at once
Policy       POLICY          = fifo;             //Scheduling Policy (fifo, rdrn, mlfq)
unsigned int SCHEDTIME       = 100;              //Time between scheduling
string       PATH            = "./";             //Path to UNIX socket
int          POLL_TIMEOUT    = 1000;             //Timeout for poll
Scheduler    scheduler_prime;                    //Instantiation of Scheduler class

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

//Handles SIGCHLD
//This will remove a process from the table
//When the parent (server) receives the SIGCHLD signal.
void sigchld_handler(int signum){
    pid_t p;
    int status;
    while ((p=waitpid(-1, &status, WNOHANG)) > 0){
        for (uint i = 0; i < scheduler_prime.get_running().size(); i++){
            if (scheduler_prime.get_running()[i].pid == p){
                
                int indiv_turnaround_time = time(NULL) - scheduler_prime.get_running()[i].schedtime;
                int indiv_response_time   = scheduler_prime.get_running()[i].starttime - scheduler_prime.get_running()[i].schedtime;
                
                scheduler_prime.increment_dead_processes();
                scheduler_prime.add_to_total_turnaround_time(indiv_turnaround_time);
                scheduler_prime.set_average_turnaround_time();
                
                reap_log(scheduler_prime.get_running()[i], indiv_turnaround_time, indiv_response_time);

                scheduler_prime.remove_process(i);
            }
        }
    scheduler_prime.set_num_running_processes(scheduler_prime.get_num_running_processes()-1);
    }
}

//Main Function
//Contains command-line parsing, client request running,
//and the main server loop.
int main(int argc, char *argv[]) {

    vector<string> command_request;    

    //  Command Line Parsing
    string programName = argv[0];
    int argIndex = 1;
    string flag;
    while (argIndex < (argc)) {
        flag = argv[argIndex++];
        if (flag == "-h") {
            usage(programName);
            return EXIT_SUCCESS;
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
                return EXIT_FAILURE;
            }
        } else if (flag == "-t") {
            SCHEDTIME = atoi(argv[argIndex++]);
        } else if (flag == "add") {
            //  Run client
	    command_request.push_back(flag);
            while (argIndex < (argc)){
	        flag = argv[argIndex++];
                command_request.push_back(flag);
	    }
	    client_request(command_request);
            return EXIT_SUCCESS;
        } else if ( flag == "status" || flag == "running" ||
                    flag == "waiting" || flag == "flush" ) {
            //  Run client
            command_request.push_back(flag);
            client_request(command_request);
            return EXIT_SUCCESS;
        } else {
            usage(programName);
            return EXIT_FAILURE;
        }
    }
    
    int server_fd = server_create();	//Starts up the server so that client can write to it
    
    scheduler_prime.setSchedulerVals(POLICY, NCPUS, SCHEDTIME);	//Sets up scheduler basics
    
    cout << "[" << time(NULL) << "] " << "INFO  Starting Process Queue Server..." << endl;
    
    //Main server loop
    //This is where the server will do everything,
    //including receiving client information and 
    //running commands. 
    while (true){

        //sigaction handler
        //This runs the signal handler
        //when the server receives a SIGCHLD signal.
        struct sigaction sa;

        sa.sa_handler = sigchld_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;                     
        sigaction(SIGCHLD, &sa, NULL);

        //This poll checks if the client is sending something to the server
        struct pollfd pfd   = {server_fd, POLLIN|POLLPRI, 0};
        int    result       = poll(&pfd, 1, POLL_TIMEOUT);

        //This poll checks if I/O is occurring
        struct pollfd pfd_1 = {STDIN_FILENO, POLLIN|POLLPRI, 0};
        int    result_1     = poll(&pfd_1, 1, SCHEDTIME/1000);

        //server_accept will occur if client tries
        //to write to server_fd
        if (result > 0){
            server_accept(server_fd);
        }

        //scheduler_prime.run() will occur when timeout occurs
        //on result_1
        if (result_1 < 0){
            fprintf(stderr, "Unable to poll: %s\n", strerror(errno));
        } else if (result_1 == 0){
            scheduler_prime.run();
        }		
    }    
    return EXIT_SUCCESS;
}
