/* pq.h
 * Matthew D'Alonzo and Jeff Klouda
 */

#include <sys/types.h>
#include <string>
#include <vector>
#include <queue>

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
    unsigned int schedtime; //  Time when process was first scheduled

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
        int executeJob (vector<string>);            // return pid/-1
        int pauseProcess (Process);
        int resumeProcess (Process);
        int terminateProcess (Process);
        void run();                                 // run scheduler
        

    private:
        int nCPUS;
        int timeSlice;
        Policy policy;
        queue<vector<string>> jobsWaiting;
        vector<Process> processTable;
};

//  ipc_communication.cpp

int server_create();
void server_accept(int);
void client_request(vector<string>);

#endif
