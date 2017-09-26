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
    time_t utime;           //  User mode CPU time, in clock ticks
    time_t stime;           //  Kernel mode CPU time, in clock ticks
    time_t cutime;          //  Waited-for children's user mode CPU time
    time_t cstime;          //  Waited-for children's kernel mode CPU time
    time_t starttime;       //  Time when process started in clock ticks
    time_t runtime;         //  Time when process was last continued
    time_t schedtime;       //  Time when process was first scheduled
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
        int  pauseProcess (Process&);
        int  resumeProcess (Process&);
        int  terminateProcess (Process&);
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
        void fifo_runner();
        void rRobin_runner();
        void mlfq_runner();
        deque<Process> get_waiting();
        deque<Process> get_running();
    private:
        unsigned int total_processes;
        unsigned int num_running_processes;
        unsigned int num_waiting_processes;
        int num_levels;
        int average_response_time;
        int average_turnaround_time;
        unsigned int nCPUS;
        int timeSlice;
        Policy policy;
        deque<Process> waiting;
        deque<Process> running;
        //vector<Process> processTable;
};

//  ipc_communication.cpp

int server_create();
void server_accept(int);
void client_request(vector<string>);

// logging.cpp
void add_log(vector<string>, int);
void status_log(int);
void running_log(int);
void waiting_log(int);
void flush_log();
void reap_log(Process);

#endif
