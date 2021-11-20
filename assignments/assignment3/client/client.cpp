/*
	C ECHO client example using sockets
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

int main(int argc , char *argv[]) {
	int sock;
	struct sockaddr_in server;
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		printf("Could not create socket");
	}

	server.sin_addr.s_addr = inet_addr("129.128.29.111");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if ( connect(sock , (struct sockaddr *)& server , sizeof(server) ) < 0) {
		perror("connect failed. Error");
		return 1;
	}

	//keep communicating with server
	while(true) {
		char message[2000];
		int read_size;
		std::vector<char> buf(5000);
		printf("Enter message : ");
		scanf("%s" , message);
		
		// Send some data
		if( send(sock , message , strlen(message) , 0) < 0) {
			puts("Send failed");
			return 1;
		}

		// read in data from server
		if( read_size = recv(sock, buf.data(), buf.size(), 0) <= 0) {
			puts("recv failed");
			break;
		}

		// print out the echoed data
		std::cout << buf.data() << std::endl;
	}
	
	close(sock);
	return 0;
}