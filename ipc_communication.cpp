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

	// Open stream from client
	//FILE *server_stream = fdopen(server_fd, "r+");
	//if (server_stream == NULL) {
	//	perror("fdopen");
	//	return;
	//}


	// Open stream from client
	
    /*
    FILE *client_stream = fdopen(client_fd, "r+");
	if (client_stream == NULL) {
		perror("fdopen");
		return;
	}
    */
	
    //char buffer[BUFSIZ];
	vector<string> command;
	//cout << "server_fd for server: " << server_fd << endl;
	//cout << "client_fd for server: " << client_fd << endl;
	//cout << "before this thing" << endl;
	//fflush(client_stream);
	
    /*
    while (fgets(buffer, BUFSIZ, client_stream)) {
		command.push_back(buffer);
	}
	fflush(client_stream);
	cout << "After this thing" << endl;
	*/
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
    //cout << "command size here: " << command.size() << endl;
	for (uint i = 0; i < command.size(); i++){
		command[i].pop_back();
	}
	if (command[0] == "add" && command.size() > 1){
		//cout << "in here" << endl;
		command.erase(command.begin());
		scheduler_prime.pushJob(command);

        
	add_log(command, client_fd);
	

        for (uint i = 0; i < command.size(); i++){
            command[i] = command[i] + "\n";
            char buffer[BUFSIZ];
            strncpy(buffer, command[i].c_str(), BUFSIZ);
            if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
                perror("send");
            }
        }
        
        scheduler_prime.set_total_processes(scheduler_prime.get_total_processes()+1);
		scheduler_prime.set_num_waiting_processes(scheduler_prime.get_num_waiting_processes()+1);
	}else if (command[0] == "status"){
		status_log();
	}else if (command[0] == "running"){
		running_log();
	}else if (command[0] == "waiting"){
		waiting_log();
	}else if (command[0] == "flush"){
		flush_log();
	}
	//fclose(server_stream);
	//fclose(client_stream);
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

	// Open stream from server
	//cout << "client_fd for client: " << client_fd << endl;
	
    /*
    FILE *server_stream = fdopen(client_fd, "r+");
	if (server_stream == NULL) {
		perror("fdopen");
		exit(EXIT_FAILURE);
	}

	//char buffer[BUFSIZ];
	string new_line = "\n";
	*/
    char  buffer[BUFSIZ];
    string commandNo = to_string(command.size());
    strncpy(buffer, commandNo.c_str(), BUFSIZ);
    if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
        perror("send");
    }
    for (uint i = 0; i < command.size(); i++){
        cout << "Top of loop\n";
        command[i] = command[i] + "\n";
        strncpy(buffer, command[i].c_str(), BUFSIZ);
        if (send(client_fd, buffer, BUFSIZ, 0) < 0) {
            perror("send");
        }
    }

    /*
    for (uint i = 0; i < command.size(); i++){
		//cout << "command[" << i << "]: " << command[i] << endl;
		fputs(command[i].c_str(), server_stream);
		fputs(new_line.c_str(), server_stream);
	}
    */

	//cout << "before sleep" << endl;
	//sleep(3);
	//cout << "after sleep" << endl;
	//cout << "after first" << endl;
	//fflush(server_stream);
    //char buffer[BUFSIZ];
   	//while (fgets(buffer, BUFSIZ, server_stream)) {
    //    fputs(buffer, stdout);
   	//}
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
    //cout << "after here" << endl;
	//fclose(server_stream);
	close(client_fd);


}


