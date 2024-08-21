#include "Client.h"
#include "common.h"
#include <string>


bool Client::Init() {
	FileLogger logger("logger.txt");
	readConfig("client.ini",&_client,&_port);
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		logger.log("Failed to initialized WSAStartup: " + std::to_string(WSAGetLastError()));
		logger.log("Client: " + _client);
		logger.log("Port: " + _port);
		return false;
	}

	_listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listensocket == INVALID_SOCKET) {
		logger.log("Cannot create socket: " + std::to_string(WSAGetLastError()));
		logger.log("Client: " + _client);
		logger.log("Port: " + _port);
		return false;
	}
	isInitialized = true;
	return true;
}

bool Client::SetListener() {
	FileLogger logger("logger.txt");
	if (!isInitialized) {
		logger.log("Object is not initialized: " + std::to_string(WSAGetLastError()));
		return false;
	}
	addrinfo hints, * results;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(_client.c_str(), _port.c_str(), &hints, &results);
	if (iResult != 0) {
		logger.log("Couldn't translate address: " + std::to_string(WSAGetLastError()));
		return false;
	}

	iResult = bind(_listensocket, results->ai_addr, (int)results->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(results);
		closesocket(_listensocket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(results);

	// No longer need Client socket
	iResult = listen(_listensocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(_listensocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Client Running" << std::endl;
}

bool Client::AcceptConnection() {
	FileLogger logger("logger.txt");
	if (!isInitialized) {
		logger.log("Object is not initialized: " + std::to_string(WSAGetLastError()));
		return false;
	}
	try {
		while (true) {
			std::cout << "Wating for connection..." << std::endl;
			_clientsocket = accept(_listensocket, NULL, NULL);
			if (_clientsocket == INVALID_SOCKET) {
				isConnected = false;
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(_clientsocket);
				WSACleanup();
				return isConnected;
			}
			isConnected = true;
			logger.log("Client Connected...");
		}
	}
	catch (...) {
		logger.log("Exception while Accepting Connection: " + std::to_string(WSAGetLastError()));
		isConnected = false;
		return false;
	}
	return isConnected;
}

int Client::SendData(std::string senddata) {
	FileLogger logger("logger.txt");
	try {
		if (_clientsocket == INVALID_SOCKET) {
			isConnected = false;
			return -1;
		}
		int bytesent = send(_clientsocket, senddata.c_str(), senddata.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			logger.log("Error while sending data: " + std::to_string(WSAGetLastError()));
			logger.log("Client: " + _client);
			logger.log("Port: " + _port);
			return -1;
		}
		return bytesent;
	}
	catch (...) {
		logger.log("Exception while sending data: " + std::to_string(WSAGetLastError()));
		logger.log("Client: " + _client);
		logger.log("Port: " + _port);
		return -1;
	}

}

int Client::ReceiveData(std::string& receivedData) {
	FileLogger logger("logger.txt");
	try {
		if (_clientsocket == INVALID_SOCKET) {
			isConnected = false;
			return -1;
		}
		char buffer[1024];
		int bytesRead = recv(_clientsocket, buffer, sizeof(buffer), 0);
		if (bytesRead != SOCKET_ERROR) {
			//Read data successfull
			receivedData.assign(buffer, bytesRead);
			return bytesRead;
		}
		else if (bytesRead == 0) {
			isConnected = false;
		}
		else {
			isConnected = false;
		}
	}
	catch (...) {
		logger.log("Exception while receiving  data: " + std::to_string(WSAGetLastError()));
		logger.log("Client: " + _client);
		logger.log("Port: " + _port);
		return -1;
	}
}

int Client::ReceiveData(char* buffer, int bufferSize) {
	FileLogger logger("logger.txt");
	try {
		if (_clientsocket == INVALID_SOCKET) {
			isConnected = false;
			return -1;
		}
		int bytesRead = recv(_clientsocket, buffer, bufferSize, 0);
		if (bytesRead != SOCKET_ERROR) {
			// Read data successfully
			return bytesRead;
		}
		else if (bytesRead == 0) {
			isConnected = false;
		}
		else {
			isConnected = false;
		}
	}
	catch (...) {
		logger.log("Exception while receiving data: " + std::to_string(WSAGetLastError()));
		logger.log("Client: " + _client);
		logger.log("Port: " + _port);
		return -1;
	}

	return -1; // or handle this case appropriately
}



/*******************************************/

void Client::Install() {
	std::string status = "Fail";
	Sleep(1000);
	SendData(status);
	isConnected = false;
	closesocket(_clientsocket);
}

void Client::Refresh() {
	std::string status = "Fail";
	std::string SystemInfo = getSystemInfoString();
	if (SystemInfo == "") {
		SendData(status);
	}
	std::cout << SystemInfo;
	SendData(SystemInfo);
	isConnected = false;
	closesocket(_clientsocket);
}

void Client::Upload() {
	std::string status = "Fail", ok = "ok";
	std::string filename;
	SendData(ok);
	// Receive the filename
	ReceiveData(filename);
	size_t lastBackslash = filename.find_last_of("\\");

	if (lastBackslash != std::string::npos) {
		// Extract the filename
		filename = filename.substr(lastBackslash + 1);
		std::cout << "Filename: " << filename << std::endl;
	}
	else {
		// Handle the case where no backslash is found
		std::cerr << "Invalid path format." << std::endl;
		SendData("Error: Invalid path format");
		isConnected = false;
		closesocket(_clientsocket);
		return;
	}

	if (filename.empty()) {
		SendData("Error: Empty filename");
		isConnected = false;
		closesocket(_clientsocket);
		return;
	}

	// Open the file for writing
	std::ofstream outputFile(filename, std::ios::binary);
	if (!outputFile.is_open()) {
		SendData("Error: Could not open file for writing");
		isConnected = false;
		closesocket(_clientsocket);
		return;
	}

	// Receive and write the file data
	char buffer[1024];
	int bytesRead;
	// Receive and write the file data
	while ((bytesRead = ReceiveData(buffer, sizeof(buffer))) > 0) {
		outputFile.write(buffer, bytesRead);
	}

	// Write the remaining data (if any)
	if (bytesRead < 0) {
		std::cerr << "Error receiving file data" << std::endl;
	}

	// Close the file
	outputFile.close();

	// Send the status
	SendData("Success");

	isConnected = false;
	closesocket(_clientsocket);
}


void Client::RecvCommand() {
	std::string rcvcommand, getstatus = "Success";
	while (true) {

		ReceiveData(rcvcommand);
		//std::cout << "Command Received: " << GetLastError() << std::endl;
		if (isConnected == true) {
			std::cout << "Command Received: " << rcvcommand << std::endl;
			//PostMessage(hwnd, WM_MSG_RECIVED, 0, 0);
		}
		if (rcvcommand == "Install") {
			Install();
			rcvcommand = "";
		}
		if (rcvcommand == "Upload") {
			Upload();
			rcvcommand = "";
		}
		if (rcvcommand == "Refresh") {
			Refresh();
			rcvcommand = "";
		}
	}


}

void Client::SetSystemInfo() {
	FileLogger logger("logger.txt");
	//Get Hostname
	char* name = new char[100];
	std::string notfound = "Not Found";
	ZeroMemory(name, 100);

	if (gethostname(name, 100) != 0) {
		// Handle the error
		int error = WSAGetLastError();
		logger.log("Error while getting hostname: " + std::to_string(error));
		strncpy_s(name, 100, notfound.c_str(), _TRUNCATE);
	}

	_hostname = name;
	delete[] name;
	//Get IP
	_ipaddress =  getEthernetIPAddresses();
	if (_ipaddress == "") {
		_ipaddress = notfound;
	}
}

