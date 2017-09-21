CSE.30341.FA17: Project 02
==========================

This is the documentation for [Project 02] of [CSE.30341.FA17].

Members
-------

1. Matthew D'Alonzo (mdalonz1@nd.edu)
2. Jeff Klouda (jklouda1@nd.edu)

Design
------

> 1. You will need at least two types of structures:
>
>   - **Process**: This should keep track of each job along with any accounting
>     information such as **command** and **PID**.
>
>   - **Scheduler**: This should keep track of the jobs in running and waiting
>     queues along with other information such as **policy**, **number of
>     CPUs**, and **time slice duration**.
>
>   With these in mind, consider the following questions:
>
>   - What sort of **accounting** with you need to keep track of for each
>     **Process**?
>
>	For each process, we would need to the command that runs the process, the PID of the process, when a process started running, when a process was scheduled, the state of the process (Running, Ready, or Blocked), the $time variables involved in the system, the usage the process does, and its priority (if you're doing MLFQ). 
>   - How will you compute the turnaround and response times for each
>     **Process**?
>
>	For turnaround time, you take the time the process completes and subtract from it the time the process arrives in the scheduler. For response time, you take the time the process is scheduled and subtract from it the time the process arrives in the scheduler.
>
>   - What information do you need to store in the **Scheduler**?  How will it
>     maintain a running and waiting queue?
>
>	You need to store which processes are in the scheduler that haven't been completed yet, what policy you're using, the number of CPU's you have, the time slice duration.
>
>   - How will you compute the average turnaround and response times for the
>     whole process queue?
>
>	You can compute a running total of all the turnaround and response times for each process, along with the total number of processes run, and then average it out at the end. 
>
Response.

> 2. Debugging this project will be difficult without good logging.  Because
>    timing is an important component of scheduling, it will be useful to have
>    a consistent logging mechanism that includes timestamps.
>
>   - How will you go about logging information in your program?
>
>	We will format and output the information when things happen, as we process the processes. 
>
>   - What sort of information will you log?
>
>	We will be logging any information associated with the process that is in the process in the scheduler, along with any particular functions we are running for the purpose of verifying the output of our functions. 
>

Response.

> 3. Jobs in the process queue eventually become processes which need to be
>    created, preempted, resumed, and terminated.
>
>   - How will you create a process and execute its command?
>
>	We will be using fork() and execvp() to create our processes and execute the command on them. 
>
>       Note: you will not want to use `/bin/sh -c command` in this assignment
>       since that creates two processes and you would only have direct control
>       of `/bin/sh` rather than `command`
>
>   - How will you implement preemption?  That is, how will you **stop** or
>     **pause** a running process?
>
>	We will keep track of the time and time slices of the scheduler, and when a process has used up its time slice (for RR or MLFQ), we will send a `SIGSTOP` signal, which will suspend the process.
>
>   - How will you **resume** a process that has been preempted?
>
>	To do this, we will use the `SIGCONT` signal, which will resume the process.
>
>   - How will you **terminate** an active process?
>
>	To do this, we will use the `SIGKILL` signal, which will kill the process. 
>
>   - How will you gather statistics or accounting information about each
>     process?  What will you store?
>
>	We will obtain the return value of a child process for its PID. We can get the timesstamp of the process by getting the system time at the time of the fork()/exec() and at the time the process enters the scheduler. To obtain the time of the running process, we use `/proc/[PID]/stat` to get utime and stime. The other statistics and accounting information will be gathered as the scheduler conducts its subroutines. 
>
>       [Hint](https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat)

Response.

> 4. The scheduler will need to activated under two conditions: when a process
>    dies and after some period of time (ie. time slice has expired).
>
>   - How will you trigger your scheduler when a process dies?  What must
>     happen when a process dies (consider both the **Scheduler** and the
>     **Process**)?
>
>	When a process is running, we will have the Server (parent) go into a wait(). That wait will end when the child dies, such that, the parent will continue onto the next process.
>
>   - How will you ensure your scheduler runs periodically after a time slice
>     has expired?
>
>	We will send a signal that will allow the parent to exit from the wait() and just suspend the currently running process. 
>
>       Note: you may wish to consider how your response to question 6 can help
>       with this.

Response.

> 5. The client and server need to communicate via a request and response
>    model.
>
>   - Which IPC mechanism will you use: named pipes or unix domain sockets?
>
>	We will use unix domain sockets.
>
>   - How will you utilize this IPC mechanism?
>
>	We will redirect the output of the processes from the server to the client. Also, we will pass signals from the client to the server to execute stuff and get info. 
>
>       Note: you may wish to consider this response in light of your answer in
>       question 6.

Response.

> 6. The server will need to perform its scheduling duties and process requests
>    from clients.
>
>   - How will you multiplex I/O and computation?
>
>	
>
>   - How will you ensure that your I/O will not block indefinitely?
>
>   - How will you allow events such as a child process dying interrupt your
>     I/O, but block such an event from interrupting your normal scheduling
>     functions?  Why would this be necessary?

Response.

> 7. Although FIFO is straightforward, both Round Robin and Multi-Level
>    Feedback Queue require preemption and some extract accounting.
>
>   - How will you perform preemption?  What happens to a process when it is
>     prempted?
>
>	We will perform preemption with timers and signals. When a process is preempted, it will either be suspended or continued. 
>
>   - How will MLFQ determine if a process needs to be lowered in priority?
>     What information must be tracked and how it be updated?
>
>	We will keep track of its percentage use of the time slice. If it uses it up, we will lower its priority. 
>
>   - How will MFLQ determine if a priority boost is required?
>
>	We boost all the processes to the highest queue periodically, using a voodoo constant. 
>
Response.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].

Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.




[Project 02]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project02.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
