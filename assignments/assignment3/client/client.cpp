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
#include <stdlib.h>
#include <chrono>

#include "tands.c"

using namespace std;

int main(int argc , char *argv[]) {
	string serv_ip_addr = "129.128.29.111"; // ue11.cs.ualberta.ca
	int server_port = 8888; // lucky port num
	vector<char> clientName(2000);
	string fullClientName;
	int sock;
	struct sockaddr_in server;

	double unix_timestamp = double(chrono::seconds(time(NULL)).count());

	gethostname(clientName.data(), clientName.size());
	fullClientName = clientName.data();
	fullClientName += "." + to_string(getpid());

	if (argc == 3) {
		server_port = stoi(argv[1]);
		serv_ip_addr = argv[2];
	}
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		perror("Could not create socket");
		return 1;
	}

	server.sin_addr.s_addr = inet_addr(serv_ip_addr.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);

	//Connect to remote server
	if ( connect(sock , (struct sockaddr *)& server , sizeof(server) ) < 0) {
		perror("connect failed. Error");
		return 1;
	}

	string line;
	while (getline(cin, line)) {
		if (line.at(0) == 'T') {
			// Send some data
			if( send(sock, line.c_str(), line.length(), 0) < 0 ) {
				puts("Send failed");
				return 1;
			}

			int read_size;
			vector<char> buf(5000);
			// read in data from server
			if( read_size = recv(sock, buf.data(), buf.size(), 0) <= 0) {
				puts("recv failed");
				break;
			}
		}
		else {
			int n = atoi(line.substr(1, string::npos).c_str());
			Sleep(n);
		}
	}
	
	close(sock);
	return 0;
}