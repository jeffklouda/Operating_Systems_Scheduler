/*
 *
 */

#include <vector>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include "pq.h"

Scheduler::Scheduler() {
    policy = fifo;
    nCPUS = 1;
    timeSlice = 100;
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
    jobsWaitingTable.push(job);
}

vector<string> Scheduler::popJob() {
    vector<string> job = jobsWaitingTable.front();
    jobsWaitingTable.pop();
    return job;
}

int Scheduler::executeJob (vector<string>) {
    vector<string> job = jobsWaitingTable.front();
    jobsWaitingTable.pop();
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
            wait (NULL);
            break;
    }

    return -1;
}
