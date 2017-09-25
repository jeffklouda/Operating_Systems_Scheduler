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
#include <stdio.h>

using namespace std;
extern Scheduler scheduler_prime;

void print_waiting_processes(){
	deque<Process> current_waiting_table = scheduler_prime.get_jobsWaitingTable();
	string command_to_print;
	for (uint i = 0; i < current_waiting_table.size(); i++){
		cout << setw(5) << right;
		cout << current_waiting_table[i].pid;
		cout << " ";
		cout << setw(20) << left;
		command_to_print = "";
		for (uint j = 0; j < current_waiting_table[i].command.size(); j++){
			if (j != 0){
				command_to_print += " ";
			}
			command_to_print += current_waiting_table[i].command[j];			
		}
		cout << command_to_print;
		cout << " ";
		cout << setw(8);
		cout << current_waiting_table[i].status;
		cout << " ";
		cout << setw(8);
		cout << current_waiting_table[i].user;
		cout << " ";
		cout << setw(9);
		cout << current_waiting_table[i].priority;
		cout << " ";		
		cout << setw(8);
		//TODO: Calculate USAGE
		cout << "0";
		cout << " ";
		cout << setw(9);
		cout << current_waiting_table[i].schedtime;
		cout << " ";
		cout << setw(9);
		cout << current_waiting_table[i].starttime;
		cout << endl;
	}
}

void print_running_processes(){
	vector<Process> current_process_table = scheduler_prime.get_processTable();
	string command_to_print;
	for (uint i = 0; i < current_process_table.size(); i++){
		cout << setw(5) << right;
		cout << current_process_table[i].pid;
		cout << " ";
		cout << setw(20) << left;
		command_to_print = "";
		for (uint j = 0; j < current_process_table[i].command.size(); j++){
			if (j != 0){
				command_to_print += " ";
			}
			command_to_print += current_process_table[i].command[j];			
		}
		cout << command_to_print;
		cout << " ";
		cout << setw(8);
		cout << current_process_table[i].status;
		cout << " ";
		cout << setw(8);
		cout << current_process_table[i].user;
		cout << " ";
		cout << setw(9);
		cout << current_process_table[i].priority;
		cout << " ";		
		cout << setw(8);
		//TODO: Calculate USAGE
		cout << "0";
		cout << " ";
		cout << setw(9);
		cout << current_process_table[i].schedtime;
		cout << " ";
		cout << setw(9);
		cout << current_process_table[i].starttime;
		cout << endl;
	}
}

void add_log(vector<string> command_vector, FILE* client_stream){
	string command_string = "";
	for (uint i = 0; i < command_vector.size(); i++){
		if (i != 0){
			command_string += " ";
		}
		command_string += command_vector[i];
	}
	string total_string = "[";
	total_string += to_string(time(NULL));
	total_string += "] ";
	total_string += "INFO  Added process ";
	total_string += to_string(scheduler_prime.get_total_processes());
	total_string += ": ";
	total_string += command_string;
	total_string += "\n";
	cout << total_string <<endl;
	//fputs(total_string.c_str(), client_stream);
	//cout << "HERE33" <<endl;
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
	print_running_processes();
	cout << endl;
	cout << "Waiting Queue:" << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Waiting Processes	
	print_waiting_processes();

}

void running_log(){
        cout << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Running Processes
	print_running_processes();
}

void waiting_log(){
        cout << endl;
	cout << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	//TODO: Write in loop to list Waiting Processes
	print_waiting_processes();
}

void flush_log(){
	cout << endl;
	cout << "Flushed " << scheduler_prime.get_num_running_processes() << " running and " << scheduler_prime.get_num_waiting_processes() << " waiting processes" << endl;
}
