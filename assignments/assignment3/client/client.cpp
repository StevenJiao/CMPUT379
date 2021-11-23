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

#include "../tands.c"
#include "clientwriter.cpp"

using namespace std;

int main(int argc , char *argv[]) {
	string serv_ip_addr = "129.128.29.111"; // ue11.cs.ualberta.ca
	int server_port = 8888; // lucky port num
	vector<char> clientName(2000);
	string fullClientName;
	int sock;
	struct sockaddr_in server;

	// create the host name
	gethostname(clientName.data(), clientName.size());
	fullClientName = clientName.data();
	fullClientName += "." + to_string(getpid());

	// get user input for port num and server addr
	if (argc == 3) {
		server_port = stoi(argv[1]);
		serv_ip_addr = argv[2];
	}
	
	// Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		perror("Could not create socket");
		return 1;
	}

	// setup initial server info
	server.sin_addr.s_addr = inet_addr(serv_ip_addr.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);

	// Connect to remote server
	if ( connect(sock , (struct sockaddr *)& server , sizeof(server) ) < 0) {
		perror("connect failed. Error");
		return 1;
	}
	
	// initialize our writer
	clientwriter writer = clientwriter(fullClientName, serv_ip_addr, server_port);

	// parse the stdin for work or sleep
	string line;
	int total = 0;
	while (getline(cin, line)) {
		// get the value associated to work or sleep
		int n = atoi(line.substr(1, string::npos).c_str());
		// if we have work, send it to the server
		if (line.at(0) == 'T') {
			// Send work data along with client name
			if( send(sock, (line + "-" + fullClientName).c_str(), line.length() + fullClientName.length() + 1, 0) < 0 ) {
				puts("Send failed");
				return 1;
			}
			// update the number of transactions sent
			total++;

			// log our sent message
			writer.writeToFile(line, n);
			
			// read in server response
			int read_size;
			vector<char> buf(5000);
			if( read_size = recv(sock, buf.data(), buf.size(), 0) <= 0) {
				puts("recv failed");
				break;
			}

			// convert char vector to string and log received message
			string s(buf.begin(), buf.end());
			int workNum = atoi(s.substr(1, string::npos).c_str());
			writer.writeToFile(s, workNum);
		}
		else {
			// log the sleep command
			writer.writeToFile(line, n);
			Sleep(n);
		}
	}

	// end our connection with the server
	close(sock);

	// set the total transactions sent
	writer.appendSummary(total);

	return 0;
}