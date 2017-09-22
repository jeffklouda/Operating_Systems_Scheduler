/* pq.h
 * Matthew D'Alonzo and Jeff Klouda
 */

#include <sys/types.h>
#include <string>
#include <vector>

enum Policy { fifo, rdrn, mlfq };
enum proc_status { running, ready, blocked };

struct Process {
    
    vector<string> command;
    pid_t pid;
    proc_status status;
    int priority;


};
