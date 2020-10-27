using namespace std;

#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

int createSocket(){
	int socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFD < 0){//if the socket could not be created
		perror("Error");
		exit(1);//exit the program with a status of 1
	}
	else{
		cout << "Socket created" << endl;
		return socketFD;//make socket successfully
	}
}

void findServer(sockaddr_in &serv_addr, hostent *server){
	
	string input1;
	string input2;
	
	do{
		cout << "Enter server: ";
		getline(cin, input1);
	}while(input1 == "");
	
	server = gethostbyname(input1.c_str());	
	
	do{
		cout << "Enter port number: ";
		getline(cin, input2);
	}while(input2 == "");
	int portno = stoi(input2);
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
	return;
}

void connectToServer(int socketFD, sockaddr_in serv_addr){
	cout << "Connecting to server..." << endl;
	int connectCheck = connect(socketFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(connectCheck < 0){//if connection could not be made
		perror("Error");
		exit(2);//exit the program with a status of 2
	}
	cout << "Connected to server" << endl;
	return;//connected successfully
}


int main(){
	struct sockaddr_in serv_addr;
    struct hostent *server;
	char buffer[256];
	
	
	int socketFD = createSocket();
	findServer(serv_addr, server);
	connectToServer(socketFD, serv_addr);
	
	string input = "";
	
	while(input != "!quit"){
		//get data from server
		char msg[1000];
		bzero(msg, sizeof(msg));
		int checkRead;
		checkRead = read(socketFD, (char*)&msg, sizeof(msg));//reads data from server
		if(checkRead < 0){//if there was an error reading data
			perror("Error");
		}
		else{
			printf("From server: %s\n",msg);//output message received
		}
		bzero(msg, sizeof(msg));//deletes message just outputted from buffer
		//send data to server
		cout << "Enter Message: ";
		getline(cin, input);
		strcpy(msg, input.c_str());
		int checkWrite = write(socketFD, (char*)&msg, strlen(msg));//writes data to server
		if(checkWrite < 0){//if there was an error writing data
			perror("Error");
		}
		bzero(msg, sizeof(msg));//deletes message just sent from buffer	
	}
	cout << "Exiting client" << endl;
	close(socketFD);
	return 0;
} 