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
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include "pq.h"
using namespace std;

extern time_t prev_time;

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
	Process unrun_process = {job, 0, ready, 0, 0, 0, 0, 0, 0, time(NULL), 0, 30};
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
            this->waiting.front().pid = pid;
            this->waiting.front().starttime = time(NULL);

			struct timeval start;
			gettimeofday(&start, NULL);
            waiting.front().runtime = start.tv_usec;

            this->running.push_back(waiting.front());
            this->waiting.pop_front();
            
            //wait (NULL);
            break;
    }

    return -1;
}

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
            p.pid = pid;
            p.starttime = time(NULL);

			struct timeval start;
			gettimeofday(&start, NULL);
            p.runtime = start.tv_usec;

           
            
            //wait (NULL);
            break;
    }

    return -1;
}

int Scheduler::pauseProcess (Process &p) {
   // for (auto it = this->running.begin(); it != this->running.end(); ++it) {
    //    if (p.pid == it->pid) {
			//cout << "attempt to pause pid: " << p.pid << "\n";
            if (kill(p.pid, SIGSTOP) < 0) {
                fprintf(stdout, "Stop signal failed: %s", strerror(errno));
            }
			//cout << "PAUSUSSUSUSUED\n";
            return 0;
      //  }
   // }
   // return -1;
}

int Scheduler::resumeProcess (Process &p) {
    //for (auto it = this->waiting.begin(); it != this->waiting.end(); ++it) {
     //   if (p.pid == it->pid) {
			//cout << "resumeProcess pid: " << p.pid << endl;
            if (kill(p.pid, SIGCONT) < 0) {
                fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
            }
			struct timeval start;
			gettimeofday(&start, NULL);
            p.runtime = start.tv_usec;
            return 0;
//        }
 //   }
  //  return -1;
}

int Scheduler::terminateProcess (Process &p) {
    if (kill(p.pid, SIGKILL) < 0) {
        fprintf(stdout, "Cont signal failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;   
}

void Scheduler::run() {
	for (uint i = 0; i < this->running.size(); i++){
		string proc_pid = "/proc/";
		proc_pid += to_string(this->running[i].pid);
		proc_pid += "/stat";
		//cout << "pid: " << this->running[i].pid;
		//cout << "proc_pid string: " << proc_pid << endl;
		FILE * proc_pid_stream = fopen(proc_pid.c_str(),	"r");
		if (proc_pid_stream == NULL){
			cout << "proc_pid_stream is null\n" << endl;
		}

		long unsigned int user_time=0;
		long unsigned int s_time=0;
		long int cu_time=0;
		long int cs_time=0;
		fscanf(proc_pid_stream, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld", &user_time, &s_time, &cu_time, &cs_time);


		running[i].utime  = user_time;
		running[i].stime  = s_time;
		running[i].cutime = cu_time;
		running[i].cstime = cs_time;
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


void Scheduler::fifo_runner() {
    while (num_running_processes < nCPUS && this->jobsWaiting()) {
        this->executeJob();
    }
}

void Scheduler::rRobin_runner() {
   // move process from running queue to waiting queue
   //cout << "beginning of robin runner\n";
   if (!this->running.empty()) {
        Process p = this->running.front();
		//cout << "p.pid: " << p.pid;
		//struct timeval stop;
		//gettimeofday(&stop, NULL);
		//cout << "ready to switch" << endl;
        if (pauseProcess(p) < 0) 
            fprintf(stdout, "pauseProcess failed\n");
		this->waiting.push_back(p);
		//cout << "waiting size: " << this->waiting.size() << endl;
		//cout << "running size: " << this->running.size() << endl;
        this->running.pop_front();
   }
  // cout << "middle of robin runner\n";
   // Move processes from waiting queue to running queue
   while (!this->waiting.empty() && this->running.size() < nCPUS) {
		//cout << "before waiting front\n";
		//cout << "pid: " << this->waiting.front().pid << endl;
        //Process p = this->waiting.front();
		//cout << "before if\n";
        if (this->waiting.front().pid == 0){
		//	cout << "after if\n";
            this->executeJob();
          //  cout << "after execute job\n";
        }else {
			//cout << "beforeresumeprocess\n";
            resumeProcess(this->waiting.front());
			//cout << "afterresumeprocess\n";
            this->running.push_back(this->waiting.front());
            this->waiting.pop_front();
        }
   }
   //cout << "end of robin runner\n";
}

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
		cout << "PRIORITYBOOST!!!!\n";
		prev_time = time(NULL);
		for (uint i = 0; i < 8; i++){
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

deque<Process> Scheduler::get_level(int index){
	return levels[index];
}
