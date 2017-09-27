/* client.cpp
 * This is the client part of the program.
 * It will send and receive messages from the server.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

#include "pq.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <poll.h>

#include <iostream>

extern Scheduler scheduler_prime;

using namespace std;

#define streq(a, b) (strcmp(a, b) == 0)

#define SOCKET_PATH "pq.mdalonz1"

int server_create(){
	vector<string> messages;

	// Create socket
	int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Bind socket
	struct sockaddr_un server_addr { .sun_family = AF_UNIX };
	strcpy(server_addr.sun_path, SOCKET_PATH);
	socklen_t server_len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);
	if (bind(server_fd, (struct sockaddr *)&server_addr, server_len) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Listen on socket
	if (listen(server_fd, SOMAXCONN) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return server_fd;
}

void server_accept(int server_fd){
	

	 // Accept client
     struct sockaddr_un client_addr;
     socklen_t client_len = sizeof(struct sockaddr_un);
     int       client_fd  = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

     if (client_fd < 0) {
         perror("accept");
         return;
     }

	vector<string> command;

    char buffer[BUFSIZ];
    if (recv(client_fd, buffer, BUFSIZ, 0) < 0) {
        perror("recv");
    }
    int commandNo = atoi(buffer);
    for (int i = 0; i < commandNo; ++i) {
        int n = recv(client_fd, buffer, BUFSIZ, 0);
        if (n < 0)
            perror("recv");
        else {
            command.push_back(buffer);
        }
    }
	for (uint i = 0; i < command.size(); i++){
		command[i].pop_back();
	}
	if (command[0] == "add" && command.size() > 1){
		command.erase(command.begin());
		scheduler_prime.pushJob(command);
	    add_log(command, client_fd);        
        scheduler_prime.set_total_processes(scheduler_prime.get_total_processes()+1);
		scheduler_prime.set_num_waiting_processes(scheduler_prime.get_num_waiting_processes()+1);
	}else if (command[0] == "status"){
		status_log(client_fd);
	}else if (command[0] == "running"){
		running_log(client_fd);
	}else if (command[0] == "waiting"){
		waiting_log(client_fd);
	}else if (command[0] == "flush"){
		flush_log();
        scheduler_prime.flush_jobs();
	}

	close(client_fd);
}

void client_request(vector<string> command){
	struct sockaddr_un server_addr { .sun_family = AF_UNIX };
	strcpy(server_addr.sun_path, SOCKET_PATH);
	size_t server_len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);

    // Create socket
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

	// Connect to server
	if (connect(client_fd, (struct sockaddr *)&server_addr, server_len) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

    char  buffer[BUFSIZ];
    string commandNo = to_string(command.size());
    strncpy(buffer, commandNo.c_str(), BUFSIZ);
    if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
        perror("send");
    }
    for (uint i = 0; i < command.size(); i++){
        command[i] = command[i] + "\n";
        strncpy(buffer, command[i].c_str(), BUFSIZ);
        if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
            perror("send");
        }
    }

    bool done = false;
    while (!done) {
        char buffer[BUFSIZ];
        int n = recv(client_fd, buffer, BUFSIZ, 0);
        if (n < 0) perror("server recv");
        else if (n == 0) {
            done = true;
        }
        else {
            cout << buffer;
        }
    }
	close(client_fd);


}


