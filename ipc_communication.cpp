/* client.cpp
 * This is the client part of the program.
 * It will send and receive messages from the server.
 * Authors: Matthew D'Alonzo and Jeff Klouda
 */

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

using namespace std;

#define streq(a, b) (strcmp(a, b) == 0)

#define SOCKET_PATH "parrot.socket"

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
	FILE *client_stream = fdopen(client_fd, "r+");
	if (client_stream == NULL) {
		perror("fdopen");
		return;
	}

	// Parrot input
	char buffer[BUFSIZ];
	while (fgets(buffer, BUFSIZ, client_stream)) {
		fputs(buffer, client_stream);
	}

	fclose(client_stream);
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
	FILE *server_stream = fdopen(client_fd, "r+");
	if (server_stream == NULL) {
		perror("fdopen");
		exit(EXIT_FAILURE);
	}

	//char buffer[BUFSIZ];

	for (uint i = 0; i < command.size(); i++){
		fputs(command[i].c_str(), server_stream);
	}

	fclose(server_stream);
	close(client_fd);


}


