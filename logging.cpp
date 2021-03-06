/* logging.cpp
 * This file contains the logging functions
 * that the server will send when the client
 * requests them.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

//Necessary Includes

#include "pq.h"

#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>

using namespace std;

extern Scheduler scheduler_prime;

string print_waiting_processes(){

    deque<Process> current_waiting_table = scheduler_prime.get_waiting();

    string command_to_print;
    stringstream ss;	

    for (uint i = 0; i < current_waiting_table.size(); i++){
        ss << setw(5) << right << current_waiting_table[i].pid;
        ss << " ";
        ss << setw(20) << left;
        command_to_print = "";
        for (uint j = 0; j < current_waiting_table[i].command.size(); j++){
            if (j != 0){
                command_to_print += " ";
            }
            command_to_print += current_waiting_table[i].command[j];			
        }

        if (command_to_print.size() > 14){
            command_to_print.erase(command_to_print.begin(), command_to_print.begin()+command_to_print.size()-14);
            command_to_print = "(...)" + command_to_print;
        }
        ss << command_to_print;
        ss << " ";
        ss << setw(8);
    
        switch (current_waiting_table[i].status_char){
            case ('A'): ss << "Waiting";
                        break;
            case ('R'): ss << "Running";
                        break;
            case ('S'): ss << "Sleeping";
                        break;
            case ('D'): ss << "Waiting";
                        break;
            case ('Z'): ss << "Zombie";
                        break;
            case ('T'): ss << "Stopped";
                        break;
            case ('t'): ss << "Tracing";
                        break;
            case ('X'): ss << "Dead";
                        break;
            case ('x'): ss << "Dead";
                        break;
            case ('K'): ss << "Wakekill";
                        break;
            case ('W'): ss << "Waking";
                        break;
            case ('P'): ss << "Parked";
                        break;
            default:    ss << "Unknown";
                        break;
        }
        ss << " ";
        ss << setw(8);
        ss << current_waiting_table[i].utime;
        ss << " ";
        ss << setw(9);
        ss << current_waiting_table[i].priority;
        ss << " ";		
        ss << setw(8);
        ss << "0";
        ss << " ";
        ss << setw(9);
        ss << current_waiting_table[i].schedtime;
        ss << " ";
        ss << setw(9);
        ss << current_waiting_table[i].starttime;
        ss << endl;
    }
    string ss_string = ss.str();
    return ss_string;
}

string print_waiting_processes_mlfq(){
	string ss_string = "";

	for (uint l = 0; l < 8; l++){

		deque<Process> current_waiting_table = scheduler_prime.get_level(l);

		string command_to_print;
		stringstream ss;	

        ss << "Level " << to_string(l) << endl;
        ss << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;	

		for (uint i = 0; i < current_waiting_table.size(); i++){
			ss << setw(5) << right;
			ss << current_waiting_table[i].pid;
			ss << " ";
			ss << setw(20) << left;
			command_to_print = "";
			for (uint j = 0; j < current_waiting_table[i].command.size(); j++){
				if (j != 0){
					command_to_print += " ";
				}
				command_to_print += current_waiting_table[i].command[j];			
			}
            
            if (command_to_print.size() > 14){
                command_to_print.erase(command_to_print.begin(), command_to_print.begin()+command_to_print.size()-14);
                command_to_print = "(...)" + command_to_print;
            }
            ss << command_to_print;
            ss << " ";
            ss << setw(8);
            
        switch (current_waiting_table[i].status_char){
            case ('A'): ss << "Waiting";
                        break;
            case ('R'): ss << "Running";
                        break;
            case ('S'): ss << "Sleeping";
                        break;
            case ('D'): ss << "Waiting";
                        break;
            case ('Z'): ss << "Zombie";
                        break;
            case ('T'): ss << "Stopped";
                        break;
            case ('t'): ss << "Tracing";
                        break;
            case ('X'): ss << "Dead";
                        break;
            case ('x'): ss << "Dead";
                        break;
            case ('K'): ss << "Wakekill";
                        break;
            case ('W'): ss << "Waking";
                        break;
            case ('P'): ss << "Parked";
                        break;
            default:    ss << "Unknown";
                        break;


        }


			ss << " ";
			ss << setw(8);
			ss << current_waiting_table[i].utime;	
			ss << " ";
			ss << setw(9);
			ss << current_waiting_table[i].priority;
			ss << " ";		
			ss << setw(8);
            ss << "0";

			ss << " ";
			ss << setw(9);
			ss << current_waiting_table[i].schedtime;
			ss << " ";
			ss << setw(9);
			ss << current_waiting_table[i].starttime;
			ss << endl;
		}
		ss_string += ss.str();
	}		
	return ss_string;
}

string print_running_processes(){
	deque<Process> current_process_table = scheduler_prime.get_running();

	string command_to_print;
	stringstream ss;
	for (uint i = 0; i < current_process_table.size(); i++){
		ss << setw(5) << right;
		ss << current_process_table[i].pid;
		ss << " ";
		ss << setw(20) << left;
		command_to_print = "";
		for (uint j = 0; j < current_process_table[i].command.size(); j++){
			if (j != 0){
				command_to_print += " ";
			}
			command_to_print += current_process_table[i].command[j];			
		}

        if (command_to_print.size() > 14){
            command_to_print.erase(command_to_print.begin(), command_to_print.begin()+command_to_print.size()-14);
            command_to_print = "(...)" + command_to_print;
        }
        

		ss << command_to_print;
		ss << " ";
		ss << setw(8);

        switch (current_process_table[i].status_char){
            case ('A'): ss << "Waiting";
                        break;
            case ('R'): ss << "Running";
                        break;
            case ('S'): ss << "Sleeping";
                        break;
            case ('D'): ss << "Waiting";
                        break;
            case ('Z'): ss << "Zombie";
                        break;
            case ('T'): ss << "Stopped";
                        break;
            case ('t'): ss << "Tracing";
                        break;
            case ('X'): ss << "Dead";
                        break;
            case ('x'): ss << "Dead";
                        break;
            case ('K'): ss << "Wakekill";
                        break;
            case ('W'): ss << "Waking";
                        break;
            case ('P'): ss << "Parked";
                        break;
            default:    ss << "Unknown";
                        break;


        }


		ss << " ";
		ss << setw(8);
		ss << current_process_table[i].utime;
		ss << " ";
		ss << setw(9);
		ss << current_process_table[i].priority;
		ss << " ";		
		ss << setw(8);
		//TODO: Calculate USAGE
        float usage = 0;
        if (time(NULL)-current_process_table[i].starttime != 0){
            usage = current_process_table[i].utime / (time(NULL) - current_process_table[i].starttime);
        }
        ss << to_string(usage);

		ss << " ";
		ss << setw(9);
		ss << current_process_table[i].schedtime;
		ss << " ";
		ss << setw(9);
		ss << current_process_table[i].starttime;
		ss << endl;
	}
	string ss_string = ss.str();
	return ss_string;
}

void add_log(vector<string> command_vector, int client_fd){
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
		
	cout << total_string;
	//char buffer[BUFSIZ];
    //strncpy(buffer, total_string.c_str(), BUFSIZ);
    //if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
    //	perror("send");
    //}

}

void status_log(int client_fd){
	string ss_string = "";
	stringstream ss;    
	ss << endl;	
    ss << "Running =";
	ss << setw(5);
	ss << scheduler_prime.get_num_running_processes();
	ss << ", Waiting =";
	ss << setw(5);
	ss << scheduler_prime.get_num_waiting_processes();
	ss << ", Levels =";
	ss << setw(5);
	ss << scheduler_prime.get_num_levels();
	ss << ", Turnaround =";
	ss << setw(5);
    ss << scheduler_prime.get_average_turnaround_time();
	ss << ", Response =";
	ss << setw(5);
	ss << scheduler_prime.get_average_response_time();
	ss << endl;	
	ss << endl;
	ss << "Running Queue:" << endl;
	ss << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " << endl;
	ss_string += ss.str();

	ss_string += print_running_processes();
	//cout << endl;

	stringstream ll;
	ll << "Waiting Queue:" << endl;
	ll << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	ss_string += ll.str();
	if (scheduler_prime.get_policy() == mlfq){
		ss_string += print_waiting_processes_mlfq();
	}else{
		ss_string += print_waiting_processes();
	}
	char buffer[BUFSIZ];
    strncpy(buffer, ss_string.c_str(), BUFSIZ);
    if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
    	perror("send");
    }

}

void running_log(int client_fd){
	string ss_string = "";
	stringstream ss;
    ss << endl;
	ss << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
	ss_string += ss.str();
	ss_string += print_running_processes();
	char buffer[BUFSIZ];
    strncpy(buffer, ss_string.c_str(), BUFSIZ);
    if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
    	perror("send");
    }
}

void waiting_log(int client_fd){
	string ss_string = "";
	stringstream ss;
    ss << endl;
	if (scheduler_prime.get_policy() != mlfq){
	    ss << "  PID COMMAND              STATE    USER     THRESHOLD USAGE    ARRIVAL    START     " 
<< endl;
    }
	ss_string += ss.str();
	if (scheduler_prime.get_policy() == mlfq){
		ss_string += print_waiting_processes_mlfq();
	}else{
		ss_string += print_waiting_processes();
	}

	char buffer[BUFSIZ];
    strncpy(buffer, ss_string.c_str(), BUFSIZ);
    if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
    	perror("send");
    }

}

void flush_log(){
	cout << endl;
	cout << "Flushed " << scheduler_prime.get_num_running_processes() << " running and " << scheduler_prime.get_num_waiting_processes() << " waiting processes" << endl;
}

//reap_log
//Sends log when process is ended
void reap_log(Process reaped_process, int t_turnaround_time, int t_response_time){
    string process_string = "";
    for (uint i = 0; i < reaped_process.command.size(); i++){
        if (i != 0){
            process_string += " ";
        }
        process_string += reaped_process.command[i];
    }
	cout << "[" << time(NULL) << "] INFO  Reaped Process " << reaped_process.pid << ": " << process_string << ", Turnaround= " << t_turnaround_time <<  ", Reponse= " << t_response_time << endl;
}


//start_process_log
//Sends log when process is first executed
void start_process_log(Process started_process){
    string process_string = "";
    for (uint i = 0; i < started_process.command.size(); i++){
        if (i != 0){
            process_string += " ";
        }
        process_string += started_process.command[i];
    }
	cout << "[" << time(NULL) << "] INFO  Started Process " << started_process.pid << ": " << process_string << endl;
}
