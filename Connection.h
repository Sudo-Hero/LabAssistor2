#pragma once
#ifndef _CONNECTION_
#define _CONNECTION_


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
class Connection {
private:
	const std::string _server;
	const std::string _port;
	SOCKET _socket;
	WSADATA ws {  };

	bool isInitialized = false;
	bool isConnected = false;
public:
	Connection(){}

	Connection(std::string server, std::string port) :
		_server{ server },
		_port{ port }
	{}
	bool Init();
	bool Connect();
	int SendData(std::string);
	int ReceiveData(std::string& receivedData);

	std::string getServer() const;
	std::string getPort() const;

	std::string ParseData(std::string& receivedData);
	~Connection() {
		WSACleanup();
		closesocket(_socket);
	}
};

#endif // !_CONNECTION_