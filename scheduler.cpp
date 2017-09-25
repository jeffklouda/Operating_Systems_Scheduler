/*
 *
 */

#include <time.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "pq.h"

Scheduler::Scheduler() {
    policy = fifo;
    nCPUS = 1;
    timeSlice = 100;
    total_processes = 0;
    num_running_processes = 0;
    num_waiting_processes = 0;
    num_levels = 8;
    average_turnaround_time = 0;
    average_response_time = 0;
}

Scheduler::~Scheduler() {

}

void Scheduler::setSchedulerVals(Policy p, int ncpus, int tmslice) {
    policy = p;
    nCPUS = ncpus;
    timeSlice = tmslice;
}

bool Scheduler::jobsWaiting() {
    return (!jobsWaitingTable.empty());
}

void Scheduler::pushJob (vector<string> job) {
	Process unrun_process = {job, 0, ready, -1, 0, 0, 0, 0, 0, time(NULL), 0};
    jobsWaitingTable.push_front(unrun_process);
}

vector<string> Scheduler::popJob() {
    vector<string> job = jobsWaitingTable.front().command;
    jobsWaitingTable.pop_front();
    return job;
}

//executeJob()
//This does the fork() and exec() on jobs
//in the waiting queue.
int Scheduler::executeJob () {
    
    vector<string> job = jobsWaitingTable.front().command;
    processTable.push_back(jobsWaitingTable.front());
    jobsWaitingTable.pop_front();

    vector<char*> exec_command;
    for (uint i = 0; i < job.size(); i++){
        exec_command.push_back(const_cast<char*>(job[i].c_str()));
    }
    exec_command.push_back(NULL);
    char **command_char = &exec_command[0];

    num_waiting_processes--;
    num_running_processes++;
    pid_t pid = fork();
    
    switch (pid) {
        case -1:            //  Error
            fprintf(stderr, "Unable to fork: %s\n", strerror(errno));
            return -1;
            break;

        case 0:             // Child
            execvp(command_char[0], command_char);
            return -1;      // fail
            break;

        default:            // Parent
            //wait (NULL);
            break;
    }

    return -1;
}

Policy Scheduler::get_policy(){
	return policy;
}

int Scheduler::get_nCPUS(){
	return nCPUS;
}

int Scheduler::get_total_processes(){
	return total_processes;
}

void Scheduler::set_total_processes(int num){
	total_processes = num;
}

int Scheduler::get_num_running_processes(){
	return num_running_processes;
}

void Scheduler::set_num_running_processes(int num){
	num_running_processes = num;
}

int Scheduler::get_num_waiting_processes(){
	return num_waiting_processes;
}

void Scheduler::set_num_waiting_processes(int num){
	num_waiting_processes = num;
}

int Scheduler::get_num_levels(){
	return num_levels;
}

void Scheduler::set_num_levels(int num){
	num_levels = num;
}

int Scheduler::get_average_turnaround_time(){
	return average_turnaround_time;
}

void Scheduler::set_average_turnaround_time(int num){
	average_turnaround_time = num;
}

int Scheduler::get_average_response_time(){
	return average_response_time;
}

void Scheduler::set_average_response_time(int num){
	average_response_time = num;
}

deque<Process> Scheduler::get_jobsWaitingTable(){
	return jobsWaitingTable;
}

vector<Process> Scheduler::get_processTable(){
	return processTable;
}
