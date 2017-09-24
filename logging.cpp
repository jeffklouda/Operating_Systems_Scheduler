/* logging.cpp
 * This file contains the logging functions
 * that the server will send when the client
 * requests them.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

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
	cout << "Added process " << scheduler_prime.get_total_processes() << ": " << command_string << endl;
}

void status_log(){


}

void running_log(){


}

void waiting_log(){


}

void flush_log(){

}
