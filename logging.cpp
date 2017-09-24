/* logging.cpp
 * This file contains the logging functions
 * that the server will send when the client
 * requests them.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

#include <time.h>
#include "pq.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
extern Scheduler scheduler_prime;

void add_log(vector<string> command_vector){
	string command_string = "";
	for (uint i = 0; i < command_vector.size(); i++){
		if (i != 0){
			command_string += " ";
		}
		command_string += command_vector[i];
	}
	cout << "[" << time(NULL) << "] " << "INFO  Added process " << scheduler_prime.get_total_processes() << ": " << command_string << endl;
}

void status_log(){
        cout << endl;	
        cout << "Running =";
	cout << setw(5);
	cout << scheduler_prime.get_num_running_processes();
	cout << ", Waiting =";
	cout << setw(5);
	cout << scheduler_prime.get_num_waiting_processes();
	cout << ", Levels =";
	cout << setw(5);
	cout << scheduler_prime.get_num_levels();
	cout << ", Turnaround =";
	cout << setw(5);
        cout << scheduler_prime.get_average_turnaround_time();
	cout << ", Response =";
	cout << setw(5);
	cout << scheduler_prime.get_average_response_time();
	cout << endl;	
	cout << endl;
	cout << "Running Queue:" << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " << endl;
	//TODO: Write in loop to list Running Processes
	cout << endl;
	cout << "Waiting Queue:" << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Waiting Processes	

}

void running_log(){
        cout << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Running Processes
}

void waiting_log(){
        cout << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Waiting Processes
}

void flush_log(){
	cout << endl;
	cout << "Flushed " << scheduler_prime.get_num_running_processes() << " running and " << scheduler_prime.get_num_waiting_processes() << " waiting processes" << endl;
}
