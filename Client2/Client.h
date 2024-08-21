#pragma once
#ifndef _client_
#define _client_

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Logger.h"

#define MAXBUFFER 1024


/********************************
SendData, ReceiveData :-
1. If successfull return > 0
2. If successfull but 0 bytes sent(return == 0)
3. If unsuccessful(return < 0 i.e, return == -1)
*********************************/
class Client {
private:
	std::string _client;
	std::string _port = "27015";
	
	SOCKET _listensocket = INVALID_SOCKET, _clientsocket = INVALID_SOCKET;
	WSADATA ws{  };

	bool isInitialized = false;
	
public:
	std::string _hostname;
	std::string _ipaddress;

	bool isConnected = false;
	Client() {}

	bool Init();
	bool AcceptConnection();
	bool SetListener();
	int SendData(std::string);
	int ReceiveData(std::string& receivedData);
	int ReceiveData(char* buffer, int bufferSize);

	void Install();
	void Upload();
	void RecvCommand();
	void Refresh();

	void SetSystemInfo();
	void SetClient();

	~Client() {
		WSACleanup();
		closesocket(_listensocket);
	}
};

#endif // !_CONNECTION_