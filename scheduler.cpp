/* scheduler.cpp
 * Implementation of functions in Scheduler class.
 * Authors: Jeff Klouda and Matthew D'Alonzo
 */

//Necessary Includes

#include "pq.h"

#include <time.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>

using namespace std;

extern time_t prev_time;

//Scheduler Constructor
Scheduler::Scheduler() {
    total_executed_processes = 0;
    policy = fifo;
    nCPUS = 1;
    timeSlice = 100;
    total_processes = 0;
    num_running_processes = 0;
    num_waiting_processes = 0;
    num_levels = 8;
    average_turnaround_time = 0;
    average_response_time = 0;
    total_turnaround_time = 0;
    total_response_time = 0;
}

//Scheduler Deconstructor
Scheduler::~Scheduler() {
}

//setSchedulerVals
//Sets constants for Scheduler
void Scheduler::setSchedulerVals(Policy p, int ncpus, int tmslice) {
    policy = p;
    nCPUS = ncpus;
    timeSlice = tmslice;
    switch (policy){
        case fifo: num_levels = 1;
                   break;
        case rdrn: num_levels = 1;
                   break;
        case mlfq: num_levels = 8;
                   break;
    }
}

//jobsWaiting
//Checks if Jobs are in waiting deque
bool Scheduler::jobsWaiting() {
    return (!waiting.empty());
}

//pushJob
//Pushes a job to the waiting deque
//when the client first sends it.
void Scheduler::pushJob (vector<string> job) {
    Process unrun_process = {
                             job,            //vector<string> command; 
                             0,              //pid_t pid;
                             ready,          //proc_status status;
                             0,              //int priority;
                             0,              //time_t utime;
                             0,              //time_t stime;
                             0,              //time_t cutime;
                             0,              //time_t cstime
                             0,              //time_t starttime;
                             time(NULL),     //time_t runtime
                             time(NULL),     //suseconds_t schedtime;
                             30,             //int user;
                             0,              //int allotment
                             0,              //float response_time;
                             0,              //int turnaround_time;
                             'A'             //char status_char;
                           };
    waiting.push_front(unrun_process);
}

//popJob
//Removes a job from the front of the waiting deque
vector<string> Scheduler::popJob() {
    vector<string> job = waiting.front().command;
    waiting.pop_front();
    
   return job;
}

//remove_process
//Removes a process from the process table
Process Scheduler::remove_process(int index){
    Process removed_process = this->running[index];
    this->running.erase(this->running.begin()+index);
    
   return removed_process;
}

//executeJob
//This does the fork() and exec() on jobs
//in the waiting queue.
//NOTE: executeJob only occurs if the policy is set to rdrn or fifo.
//NOTE: mlfq will use MLFQexecuteJob
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
            total_response_time += (time(NULL) - this->waiting.front().schedtime);
            total_executed_processes++;


            average_response_time = float(total_response_time) / float(total_executed_processes);
			
            this->waiting.front().pid = pid;
            this->waiting.front().starttime = time(NULL);

            struct timeval start;
            gettimeofday(&start, NULL);
            waiting.front().runtime = start.tv_usec;

            start_process_log(this->waiting.front());

            this->running.push_back(waiting.front());
            this->waiting.pop_front();
            
            break;
    }

    return -1;
}

//MLFQexecuteJob
//This will execute a job
//for the mlfq policy.
int Scheduler::MLFQexecuteJob (Process &p) {
    
    vector<string> job = p.command;

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
            p.response_time = (time(NULL) - p.schedtime);
            total_response_time += (time(NULL) - p.schedtime);
            total_executed_processes++;

            average_response_time = float(total_response_time) / float(total_executed_processes);

            p.pid = pid;
            p.starttime = time(NULL);
            struct timeval start;
            gettimeofday(&start, NULL);
            p.runtime = start.tv_usec;
            
            start_process_log(this->waiting.front());

            break;
    }
    return -1;
}

//pauseProcess
//This sends the SIGSTOP signal to pause a process.
int Scheduler::pauseProcess (Process &p) {
    if (kill(p.pid, SIGSTOP) < 0) {
        fprintf(stdout, "Stop signal failed: %s", strerror(errno));
    }
    return 0;
}

//resumeProcess
//This sends the SIGCONT signal to resume a process.
int Scheduler::resumeProcess (Process &p) {
    if (kill(p.pid, SIGCONT) < 0) {
        fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
    }
    struct timeval start;
    gettimeofday(&start, NULL);
    p.runtime = start.tv_usec;
    return 0;
}

//terminateProcess
//This sends the SIGKILL signall to end a process.
int Scheduler::terminateProcess (Process &p) {
    if (kill(p.pid, SIGKILL) < 0) {
        fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;   
}

//run
//This runs the scheduling policy.
void Scheduler::run() {
    for (uint i = 0; i < this->running.size(); i++){
        string proc_pid = "/proc/";
        proc_pid += to_string(this->running[i].pid);
        proc_pid += "/stat";
        
        FILE * proc_pid_stream = fopen(proc_pid.c_str(), "r");

        if (proc_pid_stream == NULL){
            cout << "proc_pid_stream is null\n" << endl;
        }

		
        fscanf(proc_pid_stream, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld", &running[i].status_char, &running[i].utime, &running[i].stime, &running[i].cutime, &running[i].cstime);
	}

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

//fifo_runner
//runs the fifo policy scheduler
void Scheduler::fifo_runner() {
    while (num_running_processes < nCPUS && this->jobsWaiting()) {
        this->executeJob();
    }
}

//rRobin_runner
//runs the round robin policy scheduler
void Scheduler::rRobin_runner() {
    if (!this->running.empty()) {
        Process p = this->running.front();
        if (pauseProcess(p) < 0) 
            fprintf(stdout, "pauseProcess failed\n");
        this->waiting.push_back(p);
        this->running.pop_front();
   }
   while (!this->waiting.empty() && this->running.size() < nCPUS) {
        if (this->waiting.front().pid == 0){
            this->executeJob(); 
        }else {
            resumeProcess(this->waiting.front());
            this->running.push_back(this->waiting.front());
            this->waiting.pop_front();
        }
   }
}

//mlfq_runner
//runs the mlfq policy scheduler
void Scheduler::mlfq_runner() {
    Process p;	
    if (!this->running.empty()){
        p = this->running.front();
        this->running.pop_front();
        if (p.utime > p.allotment){
            p.allotment+=30;
            p.priority = min(p.priority+1, num_levels-1);
        }
        pauseProcess(p);
        this->levels[p.priority].push_back(p);
    }
    if (time(NULL) != prev_time && time(NULL) % 5 == 0){
        prev_time = time(NULL);
        for (uint i = 1; i < 8; i++){
            while (!levels[i].empty()){
                p = levels[i].front();
                p.priority = 0;
                levels[i].pop_front();
                levels[0].push_back(p);
            }
        }
    }
    while (!this->waiting.empty()){
        p = this->waiting.front();
        this->waiting.pop_front();
        levels[0].push_back(p);
    }
    for (uint i = 0; i < 8; i++){	
        while (!levels[i].empty() && this->running.size() < nCPUS){
            p = levels[i].front();
            levels[i].pop_front();
            if (p.pid == 0){
                MLFQexecuteJob(p);
            }else{
                resumeProcess(p);
            }
            this->running.push_back(p);
        }
    }
}

deque<Process> Scheduler::get_waiting(){
    return waiting;
}

deque<Process> Scheduler::get_running(){
    return this->running;
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
    return this->running.size();
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

double Scheduler::get_average_turnaround_time(){
    return average_turnaround_time;
}

void Scheduler::set_average_turnaround_time(){
    average_turnaround_time = (double)total_turnaround_time / (double)total_dead_processes;
}

double Scheduler::get_average_response_time(){
    return average_response_time;
}

void Scheduler::set_average_response_time(int num){
    average_response_time = num;
}

deque<Process> Scheduler::get_level(int index){
    return levels[index];
}

void Scheduler::increment_dead_processes(){
    total_dead_processes++;
}

void Scheduler::add_to_total_turnaround_time(unsigned int num){
    total_turnaround_time += num;
}

void Scheduler::flush_jobs(){
    while(!this->running.empty()){
        terminateProcess(this->running.front());
        //num_running_processes--;
        this->running.pop_front();
    }

    if (policy == mlfq){
        for (uint i = 0; i < 8; i++){
            while (!this->levels[i].empty()){
                if(this->levels[i].front().pid == 0){
                    this->levels[i].pop_front();
                    num_waiting_processes--;
                }else{
                    terminateProcess(this->levels[i].front());
                    this->levels[i].pop_front();
                    num_waiting_processes--;
                }
            }
        }
    }else{
        while (!this->waiting.empty()){
            if(this->waiting.front().pid == 0){
                this->waiting.pop_front();
                num_waiting_processes--;
            }else{
                terminateProcess(this->waiting.front());
                this->waiting.pop_front();
                num_waiting_processes--;
            }
        }
    }

}
