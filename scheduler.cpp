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
    return (!waiting.empty());
}

void Scheduler::pushJob (vector<string> job) {
	Process unrun_process = {job, 0, ready, -1, 0, 0, 0, 0, 0, time(NULL), 0};
    waiting.push_front(unrun_process);
}

vector<string> Scheduler::popJob() {
    vector<string> job = waiting.front().command;
    waiting.pop_front();
    return job;
}

Process Scheduler::remove_process(int index){
	Process removed_process = this->running[index];
	this->running.erase(this->running.begin()+index);
	return removed_process;
}

//executeJob()
//This does the fork() and exec() on jobs
//in the waiting queue.
int Scheduler::executeJob () {
    
    vector<string> job = waiting.front().command;

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
            waiting.front().pid = pid;
            waiting.front().starttime = time(NULL);
            waiting.front().runtime = time(NULL);
            this->running.push_back(waiting.front());
            waiting.pop_front();
            
            //wait (NULL);
            break;
    }

    return -1;
}

int Scheduler::pauseProcess (Process &p) {
    for (auto it = this->running.begin(); it != this->running.end(); ++it) {
        if (p.pid == it->pid) {
            if (kill(p.pid, SIGSTOP) < 0) {
                fprintf(stdout, "Stop signal failed: %s", strerror(errno));
            }
            return 0;
        }
    }
    return -1;
}

int Scheduler::resumeProcess (Process &p) {
    for (auto it = this->waiting.begin(); it != this->waiting.end(); ++it) {
        if (p.pid == it->pid) {
            if (kill(p.pid, SIGCONT) < 0) {
                fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
            }
            p.runtime = time(NULL);
            return 0;
        }
    }
    return -1;
}

int Scheduler::terminateProcess (Process &p) {
    if (kill(p.pid, SIGKILL) < 0) {
        fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;   
}

void Scheduler::run() {
    switch (policy) {
        case fifo:  this->fifo_runner();
                    break;
        case rdrn:  this->rRobin_runner();
                    break;
        case mlfq:  this->mlfq_runner();
                    break;
        default:    break;
    }
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

void Scheduler::fifo_runner() {
    while (num_running_processes < nCPUS && this->jobsWaiting()) {
        this->executeJob();
    }
}

void Scheduler::rRobin_runner() {
   // move process from running queue to waiting queue
   if (!this->running.empty()) {
        Process p = this->running.front();
        if (pauseProcess(p) < 0) 
            fprintf(stdout, "pauseProcess failed\n");
        this->running.pop_front();
        this->waiting.push_back(p);
        
   }
   // Move processes from waiting queue to running queue
   while (!this->waiting.empty() && this->running.size() < nCPUS) {
        Process p = this->waiting.front();
        if (p.pid == 0) 
            this->executeJob();
        else {
            resumeProcess(p);
            this->running.push_back(p);
            this->waiting.pop_front();
        }
   }

}

void Scheduler::mlfq_runner() {

}

deque<Process> Scheduler::get_waiting(){
	return waiting;
}

deque<Process> Scheduler::get_running(){
	return this->running;
}

