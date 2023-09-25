#include "common.h"
#include "Logger.h"
#include "Connection.h"
#include "Activation.h"
#include "mysql_conn.h"
#include "des/DES.h"
#include <cppconn/statement.h>
#include "sha256.h"
#include <sstream>
#include <fstream>

bool prepareForOffline(char* productKey, std::string email) {
	FileLogger logger("log.txt");
	std::string key(productKey);
	std::ofstream license("license.txt", std::ios::binary);

	if (!license.is_open()) {
		logger.log("Couldn't open license file");
		return false;
	}

	//Replase sha256 with DES
	char* cipher = nullptr;
	cipher = encrypt(key.c_str());
	if (!cipher) {
		logger.log("Invalid Token");
	}
	std:string ciphertext(cipher, std::ios::binary);
	license.write(ciphertext.c_str(), 24);

	logger.log("License written: " + ciphertext);
	delete[] cipher;
	//*******************************************
	try {
		sql::Connection* conn = startMysqlConn();
		sql::Statement* stmt = conn->createStatement();
		stmt->execute("DROP DATABASE IF EXISTS labassistor");
		stmt->execute("CREATE DATABASE  labassistor;");
		conn->setSchema("labassistor");
		stmt->execute("CREATE TABLE ADMIN(USERNAME VARCHAR(100),EMAIL VARCHAR(100)PRIMARY KEY,PASSWORD VARCHAR(100),TOKEN VARCHAR(19))");//SQHM-K11K-YWJQ-QV1D
		std::string insert = "INSERT INTO ADMIN VALUES(" + std::string(" 'admin', ")+ std::string("'") + email +std::string("'") + std::string(",") + std::string(" 'admin@123',") + std::string("'") + std::string(productKey) + std::string("'") + std::string(")");
		stmt->executeQuery(insert);
		
	}
	catch (sql::SQLException e) {
		logger.log(e.what());

	}
	return true;
}



bool submitKey(char * productKey) {
	FileLogger logger("log.txt");
	try {
		std::string server, port;
		readConfig("apple.txt", &server, &port);
		if (!server.length()) {
			logger.log("Config error");
			return false;
		}
		if (!port.length()) {
			logger.log("Config error");
			return false;
		}
		logger.log("Configuration success!!");
		logger.log("Server: " + server);
		logger.log("Port: " + port);
		Connection checkKey(server, port);
		if (!checkKey.Init()) {
			logger.log("Cannot Init Connection!!");
		}
		if (!checkKey.Connect()) {
			logger.log("Cannot Create Connection!!");
		}
		std::string key(productKey);
		logger.log(key);

		//Prepare data
		std::string path = "/CWms/api/checktoken.php?ASDER55sd6sdf4saasd1asd2dfdf5=" + key;
		std::string request = "GET " + path + " HTTP/1.1" + "\r\n";
		request += "Host: " + checkKey.getServer()+ ":"+ checkKey.getPort() + "\r\n";
		request += "Accept: */*\r\n";
		request += "Accept-Language: en-us\r\n";
		request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0\r\n";
		request += "Connection: close\r\n";
		request += "\r\n";

		if (!checkKey.SendData(request)) {
			logger.log("Cannot Send Data!!");
		}

		std::string response;
		if (!checkKey.ReceiveData(response)) {
			logger.log("Cannot Receive Data!!");
		}
		std::string result = checkKey.ParseData(response), email;
		logger.log(result);
		std::istringstream data(result);//SQHM-K11K-YWJQ-QV1D
		std::getline(std::getline(std::getline(data, result, ':'), result, ':'), email, ':');

		if (result == "SUCCESS") {
			if (!prepareForOffline(productKey, email)){
				MessageBoxA(NULL, "Something went wrong with MYSQL", "MYSQL Failed", MB_OK | MB_ICONERROR);
			}
			MessageBoxA(NULL, "The product has been activated please restart the aplication", "LabAssistor Activated!!", MB_OK | MB_ICONINFORMATION);
			PostQuitMessage(0);
			return true;
		}
		return false;
	}
	catch(...) {
		logger.log("Exception in activation!!");
		return false;
	}
}
