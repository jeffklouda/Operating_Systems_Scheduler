/* pq.h
 * Matthew D'Alonzo and Jeff Klouda
 */

#include <sys/types.h>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <stdio.h>
#include <sys/time.h>

#ifndef PQ_H
#define PQ_H

using namespace std;

enum Policy { fifo, rdrn, mlfq };
enum proc_status { running, ready, blocked };

//Process Struct
struct Process {    
    vector<string>    command;            //Command that created the process
    pid_t             pid;                //Pid of process
    proc_status       status;             //Either running, ready, or blocked
    int               priority;           //MLFQ priority
    long unsigned int utime;              //User mode CPU time, in clock ticks
    long unsigned int stime;              //Kernel mode CPU time, in clock ticks
    long int          cutime;             //Waited-for children's user mode CPU time
    long int          cstime;             //Waited-for children's kernel mode CPU time
    time_t            starttime;          //Time when process started in clock ticks
    time_t            runtime;            //Time when process was last continued
    suseconds_t       schedtime;          //Time when process was first scheduled
    int               user;               //User (UNUSED)
    int               allotment;          //Current allotment of usertime
    float             response_time;      //Response time (I think UNUSED)
    int               turnaround_time;    //Turnaround time (I think UNUSED)
};

//Scheduler class
class Scheduler {

    public:
        Scheduler ();
        ~Scheduler ();
        void            setSchedulerVals (Policy, int ncpus, int tmslice);
        Process         getProcess(pid_t);         
        bool            jobsWaiting();                                      // return true if jobs waiting
        void            pushJob (vector<string>);               
        vector<string>  popJob();
        Process         remove_process(int);
        int             executeJob ();                                      // return pid/-1
        int             pauseProcess (Process&);
        int             resumeProcess (Process&);
        int             terminateProcess (Process&);
        void            run();                                             // run scheduler
        Policy          get_policy();  
        int             get_nCPUS();    
        int             get_total_processes();
        void            set_total_processes(int);
        int             get_num_running_processes();
        void            set_num_running_processes(int);
        int             get_num_waiting_processes();
        void            set_num_waiting_processes(int);
        int             get_num_levels();
        void            set_num_levels(int);
        int             get_average_turnaround_time();
        void            set_average_turnaround_time();
        int             get_average_response_time();
        void            set_average_response_time(int);
        void            fifo_runner();
        void            rRobin_runner();
        void            mlfq_runner();
        deque<Process>  get_waiting();
        deque<Process>  get_level(int);
        deque<Process>  get_running();
        int             MLFQexecuteJob(Process&);
        void            increment_dead_processes();
        void            add_to_total_turnaround_time(unsigned int);

    private:
        unsigned int    total_processes;
        unsigned int    total_executed_processes;
        unsigned int    total_dead_processes;
        unsigned int    num_running_processes;
        unsigned int    num_waiting_processes;
        unsigned int    total_turnaround_time;
        unsigned int    total_response_time;
        int             num_levels;
        float           average_response_time;
        float           average_turnaround_time;
        unsigned int    nCPUS;
        time_t          timeSlice;
        Policy          policy;
        deque<Process>  waiting;
        deque<Process>  levels[8];
        deque<Process>  running;

};

//  ipc_communication.cpp
int  server_create();
void server_accept(int);
void client_request(vector<string>);

// logging.cpp
void add_log(vector<string>, int);
void status_log(int);
void running_log(int);
void waiting_log(int);
void flush_log();
void reap_log(Process, int, int);

#endif
