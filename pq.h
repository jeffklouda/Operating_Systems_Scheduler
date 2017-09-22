/* pq.h
 * Matthew D'Alonzo and Jeff Klouda
 */

#include <sys/types.h>
#include <string>
#include <vector>

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
        Scheduler (Policy, int ncpus, int tmslice);
        ~Scheduler ();
        Process getProcess(pid_t);         
        int addJob (string);                        // return error?
        int executeJob (string);
        int pauseProcess (Process);
        int resumeProcess (Process);
        int terminateProcess (Process);
        void run();                                 // run scheduler
        

    private:
        int nCPUS;
        int timeSlice;
        Policy policy;
        vector<Process> processTable;
};
