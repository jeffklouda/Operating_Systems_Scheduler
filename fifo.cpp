/* fifo.cpp
 * This file contains the function that
 * does fifo scheduling.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

#include "pq.h"

extern Scheduler scheduler_prime;

void fifo_runner(){
	while (scheduler_prime.get_num_running_processes() < scheduler_prime.get_nCPUS() && scheduler_prime.get_num_waiting_processes() > 0){
		scheduler_prime.executeJob();
	}
}
