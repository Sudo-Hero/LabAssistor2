#include "Connection.h"
#include <sstream>

FileLogger logger("logger.txt");

bool Connection::Init(){
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		logger.log("Failed to initialized WSAStartup: " + std::to_string(WSAGetLastError()));
		logger.log("Server: " + _server);
		logger.log("Port: " + _port);
		return false;
	}

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET) {
		logger.log("Cannot create socket: " + std::to_string(WSAGetLastError()));
		logger.log("Server: " + _server);
		logger.log("Port: " + _port);
		return false;
	}
	isInitialized = true;
	return true;
}


bool Connection::Connect() {
	try {
		if (!isInitialized) {
			logger.log("Object is not initialized: " + std::to_string(WSAGetLastError()));
			return false;
		}
		addrinfo hints, * results;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;

		int iResult = getaddrinfo(_server.c_str(), _port.c_str(), &hints, &results);
		if (iResult != 0) {
			logger.log("Couldn't translate address: " + std::to_string(WSAGetLastError()));
			return false;
		}
		if (connect(_socket, results->ai_addr, (int)results->ai_addrlen) == SOCKET_ERROR) {
			freeaddrinfo(results);
			logger.log("Couldn't connect to the server: " + std::to_string(WSAGetLastError()));
			return false;
		}
		freeaddrinfo(results);
		isConnected = true;
		return true;
	}
	catch (std::exception e) {
		logger.log("Exception while sending data: " + std::to_string(WSAGetLastError()));
		logger.log(e.what());
		return false;
	}
}

int Connection::SendData(std::string senddata) {
	try {
		if (!isConnected) {
			logger.log("Not Connected: " + std::to_string(WSAGetLastError()));
			logger.log("Server: " + _server);
			logger.log("Port: " + _port);
			return -1;
		}
		const char* f= "sd ";
		std::string dd = "sd";
		dd.assign(f);
		int bytesent = send(_socket, senddata.c_str(), senddata.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			logger.log("Error while sending data: " + std::to_string(WSAGetLastError()));
			logger.log("Server: " + _server);
			logger.log("Port: " + _port);
			return -1;
		}
		return bytesent;
	}
	catch (std::exception e) {
		logger.log("Exception while sending data: " + std::to_string(WSAGetLastError()));
		logger.log(e.what());
		logger.log("Server: " + _server);
		logger.log("Port: " + _port);
		return -1;
	}
	
}

int Connection::ReceiveData(std::string& receivedData) {
	try {
		if (!isConnected) {
			logger.log("Not Connected: " + std::to_string(WSAGetLastError()));
			logger.log("Server: " + _server);
			logger.log("Port: " + _port);
			return -1;
		}
		char buffer[1024];
		int bytesRead = recv(_socket, buffer, sizeof(buffer), 0);
		if (bytesRead == SOCKET_ERROR) {
			std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
			logger.log("Server: " + _server);
			logger.log("Port: " + _port);
			//Don't return just log the message
			return -1;
		}

		receivedData.assign(buffer, bytesRead);
		return bytesRead;
	}
	catch (std::exception e) {
		logger.log("Exception while receiving  data: " + std::to_string(WSAGetLastError()));
		logger.log(e.what());
		logger.log("Server: " + _server);
		logger.log("Port: " + _port);
		return -1;
	}
}


std::string Connection::ParseData(std::string& receivedData) {
	size_t bodyStart = receivedData.find("\r\n\r\n");
	if (bodyStart != std::string::npos) {
		bodyStart += 4; // Move past the empty line
		std::string responseBody = receivedData.substr(bodyStart);
		return responseBody;
	}
	else {
		std::cerr << "No response body found" << std::endl;
	}
}

std::string Connection::getServer() const{
	return _server;
}

std::string Connection::getPort() const{
	return _port;
}

