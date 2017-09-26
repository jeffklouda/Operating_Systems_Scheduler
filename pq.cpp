/* pq.cpp
 * Authors: Matthew D'Alonzo and Jeff Klouda
 * Multi-core process queue
 */
#include "pq.h"

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

//  Global Variables
time_t prev_time=0;
int NCPUS = 1;
Policy POLICY = fifo;
unsigned int SCHEDTIME = 100;         //  time between scheduling
string PATH = "./";
int POLL_TIMEOUT = 1000;
Scheduler scheduler_prime; 

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

void sigchld_handler(int signum){
    pid_t p;
    int status;
	//cout << "before first while\n";
    while ((p=waitpid(-1, &status, WNOHANG)) > 0){
		//cout << "before first for\n";
		sleep(1);
        for (uint i = 0; i < scheduler_prime.get_running().size(); i++){
			//cout << "before first if\n";
            if (scheduler_prime.get_running()[i].pid == p){
				 cout << "Process pid: " << scheduler_prime.get_running()[i].pid << endl;
                 reap_log(scheduler_prime.get_running()[i]);
				 scheduler_prime.increment_dead_processes();
				 scheduler_prime.add_to_total_turnaround_time(time(NULL)-scheduler_prime.get_running()[i].schedtime);
				 scheduler_prime.set_average_turnaround_time();
		 		 scheduler_prime.remove_process(i);

            }
        }
    }
	//cout << "after while\n";
    //wait(NULL);
	//cout << "after wait\n";
    scheduler_prime.set_num_running_processes(scheduler_prime.get_num_running_processes()-1);

}

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
            return 1;
        }
    }
    /*  Debug command line options
    cout << "NCPUS: " << NCPUS << endl;
    cout << "POLICY: " << POLICY << endl;
    cout << "SCHEDTIME: " << SCHEDTIME << endl;
    cout << "PATH: " << PATH << endl;
    */
	int server_fd = server_create();
	scheduler_prime.setSchedulerVals(POLICY, NCPUS, SCHEDTIME);
	cout << "[" << time(NULL) << "] " << "INFO  Starting Process Queue Server..." << endl;
	while (true){

		struct sigaction sa;


   		sa.sa_handler = sigchld_handler;
    	sigemptyset(&sa.sa_mask);
    	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; /* Restart functions if
                                 interrupted by handler */
    	sigaction(SIGCHLD, &sa, NULL);

		
		struct pollfd pfd = {server_fd, POLLIN|POLLPRI, 0};
        int    result     = poll(&pfd, 1, POLL_TIMEOUT);

		struct pollfd pfd_1 = {STDIN_FILENO, POLLIN|POLLPRI, 0};
        int    result_1     = poll(&pfd_1, 1, SCHEDTIME/1000);
		
		if (result < 0){
			fprintf(stderr, "Unable to poll: %s\n", strerror(errno));
		} else if (result > 0){
			server_accept(server_fd);
		}

		if (result_1 < 0){
			fprintf(stderr, "Unable to poll: %s\n", strerror(errno));
		} else if (result_1 == 0){
			scheduler_prime.run();
		}		

		

	}    


    return EXIT_SUCCESS;
}
