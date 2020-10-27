using namespace std;

#include <iostream>
#include <unistd.h>
#include <syscall.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>

int createSocket(){//create the socket to use for communication
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

int serverInfo(){//print the server's IP address and ask the user for a port number to use
    char output[256]; 
    char *ipAddr; 
    struct hostent *serverInfo; 
    int hostname; 
    hostname = gethostname(output, sizeof(output)); 
    serverInfo = gethostbyname(output); 
    ipAddr = inet_ntoa(*((struct in_addr*) serverInfo->h_addr_list[0])); 
    printf("Server IP: %s\n", ipAddr); 
	string portNumString;
	int portNum;
	bool portTest;
	do{
		cout << "Enter port number: ";
		getline(cin, portNumString);
		portTest = true;
		try{//makes sure the provided port is a integer
          portNum = stoi(portNumString);
		}
		catch(invalid_argument const &e){
			cout << "Port number provided not valid" << endl;
			portTest = false;
		}
		if(portNum < 1024){//makes sure the provided port is above the priviledged ports
			cout << "Port number must be above 1024" << endl;
			portTest = false;
		}
	}while(portTest == false);
	return portNum;
}

void bindSocket(int portNum, sockaddr_in serverAddress, int socketFD){//bind the created socket
	bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNum);
    if (bind(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){ 
		perror("Error");
		exit(1);//error on bind, exit with status 1
	}
	cout << "Bind complete" << endl;
	return;
}

int listenForConnection(int socketFD, sockaddr_in clientAddress){//listen for clients wanting to connect
	int listenCheck = listen(socketFD,1);
	if(listenCheck < 0){
		perror("Error");
		exit(2);//error on listen, exit with status 2
	}
	int clientLength = sizeof(clientAddress);
	cout << "Listening for connection..." << endl;
}

int acceptConnection(int socketFD, sockaddr_in clientAddress, char buffer[]){//accept connection from found client
	socklen_t clientLength = sizeof(clientAddress);
	int newSocketFD = accept(socketFD, (struct sockaddr *) &clientAddress, &clientLength);
    if (newSocketFD < 0){
		perror("Error");
	}
    bzero(buffer,256);
	cout << "Accepted connection" << endl;
	return newSocketFD;
}
/*
void getData(char buffer[], int socketFD){//read data from client
	int readCheck;
	do{
		readCheck = read(socketFD, buffer, strlen(buffer));
	}while(buffer == "");
    if (readCheck < 0){
		perror("Error");
		exit(4);//error reading from client, exit with status 4
	}
    bzero(buffer, strlen(buffer));//reset buffer to blank
	return;
}

void sendData(int socketFD){//send data to client
	char msg[1000];
	string input;
	cout << "Enter message: ";
	getline(cin, input);
	memset(&msg, 0, sizeof(msg));
    strcpy(msg, input.c_str());
	int writeCheck = write(socketFD, msg, sizeof(input));
	if(writeCheck < 0){
		perror("Error");
		exit(3);//error writing to client, exit with status 3
	}
	return;
}
*/
int main(){
	
	struct sockaddr_in serverAddress, clientAddress;
	char buffer[256];
	bzero((char *) &serverAddress, sizeof(serverAddress));
	
	int socketFD = createSocket();
	int portNum = serverInfo();
	bindSocket(portNum, serverAddress, socketFD);
	
	while(true){//keep listening for new clients as long as the program is running
		listenForConnection(socketFD, clientAddress);
		int newSocketFD = acceptConnection(socketFD, clientAddress, buffer);
		char msg[1000];
		do{//when a client is found, keep sending messages back and forth
			//send data to client
			string input;
			bzero(msg, sizeof(msg));
			cout << "Enter message: ";
			getline(cin, input);
			memset(&msg, 0, sizeof(msg));
			strcpy(msg, input.c_str());
			int writeCheck = write(newSocketFD, (char*)&msg, strlen(msg));
			if(writeCheck < 0){
				perror("Error");
				exit(3);//error writing to client, exit with status 3
			}
			bzero(msg, sizeof(msg));
			//get data from client
			int readCheck;
			readCheck = read(newSocketFD, (char*)&msg, sizeof(msg));
			if (readCheck < 0){
				perror("Error");
				exit(4);//error reading from client, exit with status 4
			}
			printf("From client: %s\n",msg);//output message received
		}while(strcmp(msg, "!quit"));//stop connection when client ends session
		cout << "Client terminated connection" << endl;
	}
	
	return 0;
}