/* pq.h
 * Matthew D'Alonzo and Jeff Klouda
 */

#include <sys/types.h>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <stdio.h>

#ifndef PQ_H
#define PQ_H

using namespace std;

enum Policy { fifo, rdrn, mlfq };
enum proc_status { running, ready, blocked };

struct Process {
    
    vector<string> command;
    pid_t pid;
    proc_status status;
    int priority;           //  MLFQ priority
    unsigned int utime;     //  User mode CPU time, in clock ticks
    unsigned int stime;     //  Kernel mode CPU time, in clock ticks
    unsigned int cutime;    //  Waited-for children's user mode CPU time
    unsigned int cstime;    //  Waited-for children's kernel mode CPU time
    unsigned int starttime; //  Time when process started in clock ticks
    time_t schedtime; //  Time when process was first scheduled
	int user;
};

class Scheduler {

    public:
        Scheduler ();
        ~Scheduler ();
        void setSchedulerVals (Policy, int ncpus, int tmslice);
        Process getProcess(pid_t);         
        bool jobsWaiting();                         // return true if jobs waiting
        void pushJob (vector<string>);               
        vector<string>  popJob();
	Process remove_process(int);
        int  executeJob ();            // return pid/-1
        int  pauseProcess (Process);
        int  resumeProcess (Process);
        int  terminateProcess (Process);
        void run();                                 // run scheduler
	Policy get_policy();  
	int  get_nCPUS();    
	int  get_total_processes();
	void set_total_processes(int);
	int  get_num_running_processes();
	void set_num_running_processes(int);
	int  get_num_waiting_processes();
	void set_num_waiting_processes(int);
	int  get_num_levels();
	void set_num_levels(int);
	int  get_average_turnaround_time();
	void set_average_turnaround_time(int);
	int  get_average_response_time();
	void set_average_response_time(int);
	deque<Process> get_jobsWaitingTable(); 
	vector<Process> get_processTable();
    private:
	int total_processes;
	int num_running_processes;
	int num_waiting_processes;
	int num_levels;
	int average_response_time;
	int average_turnaround_time;
        int nCPUS;
        int timeSlice;
        Policy policy;
        deque<Process> jobsWaitingTable;
        vector<Process> processTable;
};

//  ipc_communication.cpp

int server_create();
void server_accept(int);
void client_request(vector<string>);

// logging.cpp
void add_log(vector<string>, FILE *);
void status_log();
void running_log();
void waiting_log();
void flush_log();
void reap_log(Process);

// fifo.cpp
void fifo_runner();

#endif
