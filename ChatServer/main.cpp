#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <filesystem>

#pragma comment(lib, "ws2_32.lib")


void read_file(std::ofstream& readF, const char* messageBuffer) {
	readF << messageBuffer;
}

bool checkLS(const char* messageBuffer, unsigned int messageSize)
{
	std::string toCheck = "ls";
	int ptr = 0;
	for (size_t i = 0; i < messageSize; i++)
	{
		if (toCheck[ptr] == messageBuffer[i])
		{
			ptr += 1;
			if (ptr == toCheck.size()) { std::cout << "ls command found" << std::endl; return true; }
		}
		else { ptr = 0; }
	}

	return false;
}


int main() {

	std::cout << "Network Prorgamming Time!" << std::endl;

	// Initialize WINSOCK 	

	WSADATA wsadata;

	// MAKEWORD(2,2), send version to WSAStartup struct

	int connectionStart = WSAStartup(MAKEWORD(2, 2), &wsadata);

	// Socket object to be used by the server
	// AF_INET, IPv4 address family, SOCK_STREAM, used to specify stream socket
	// Last socket() parameter specifies some random protocol, keep 0

	SOCKET lSock = socket(AF_INET, SOCK_STREAM, 0);

	// Returns a socket value that isn't invalid
	// Confirm valid socket

	if (lSock == INVALID_SOCKET) {
		std::cout << "Socket could not be initialized" << std::endl;
		WSACleanup();
		return 0;
	}

	// Connect socket to IP address and port
	
	sockaddr_in hints;

	hints.sin_family = AF_INET;
	hints.sin_port = htons(54000);
	hints.sin_addr.S_un.S_addr = INADDR_ANY;

	// connectionStart to check if binding takes place
	// lSock, socket in context to be binded
	// The pointer to sockaddr provides the above three struct values
	// sizeof strcut

	connectionStart = bind(lSock, (sockaddr*)&hints, sizeof(hints));

	if (connectionStart != 0) {
		std::cout << "Socket could not be bound" << std::endl;
		WSACleanup();
		return 0;
	}


	// Tell the socket to listen for all incoming requests on the IP address and port it's sitting on

	if (listen(lSock, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Socket listening could not begin" << std::endl;
		std::cout << "Error Code: " << WSAGetLastError() << std::endl;		
		closesocket(lSock);
		WSACleanup();
		return 0;
	}

	// Socket's in listenng mode
	// Time to accept connection requests

	// Check client connection

	std::cout << "Client Side Activated" << std::endl;

	SOCKET cSock = INVALID_SOCKET, c2Sock = INVALID_SOCKET;

	cSock = accept(lSock, NULL, NULL);

	std::cout << "First Client In Call!" << std::endl;
	
	// getsockopt() to check if our client socket is listening

	//int data;
	//int optLen = sizeof(int);
	//getsockopt(lSock, SOL_SOCKET, SO_ACCEPTCONN, (char*)&data, &optLen);

	//std::cout << data << std::endl;

	if (cSock == INVALID_SOCKET) {
		std::cout << "Client socked unable to conenct" << std::endl;
		std::cout << "Error Code: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Connection Requested" << std::endl;
		std::cout << "Nice connection you got set up here" << std::endl;
		closesocket(lSock);
	}

	// Now that we're in the client socket, we'll just echo messages back to the client

	char mesBuf[4096];
	ZeroMemory(mesBuf, 4096);

	// Check if our file is ready to read into

	std::ofstream outP = std::ofstream("C:\\Users\\14168\\Documents\\ReadInto.txt", std::ios::app);

	if (!outP) {
		std::cout << "File Could Not Be Opened!" << std::endl;
	}


	while (true) {
		ZeroMemory(mesBuf, 4096);
		unsigned int mesBug = recv(cSock, mesBuf, 4096, 0);
		std::cout << "Buffer size pre-message: " << mesBug << std::endl;
		if (mesBug == 0) {
			std::cout << "Connection ended by client" << std::endl;
			break;
		}
		if (mesBug == SOCKET_ERROR) {
			std::cout << "Error" << std::endl;
			break;
		}

		if (strcmp("ls\r\n", mesBuf) == 0) {
			std::cout << "Found ls " << mesBuf << std::endl;
		}


		read_file(outP, mesBuf);
		std::cout << "Size of Client Message: " << mesBug << std::endl;
		send(cSock, mesBuf, 4096, 0);					
	}
	

	std::cout << "Server Shutting Down" << std::endl;


	// Close Client Socket
	closesocket(cSock);

	// WSACleanup
	WSACleanup();
	





	return 0;

}

