#include "mysql_conn.h"
#include "SignIn_Check.h"
#include "des/DES.h"
#include <fstream>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>
#include "Logger.h"
#include <windows.h>
#include <regex>
#include <sha256.h>

#define QUOTE std::string("'")

void SignIn::askForToken() {
	EnableWindow(hMain, false);
	activationbox.RegisterActClass();
	activationbox.CreateAcivationWindow(hMain);
}

void SignIn::ActivationCheck() {
	FileLogger logger("log.txt");
	std::ifstream hashfile("license.txt", std::ios::binary);
	
	if (!hashfile.is_open()) {
		//License does not exist
		logger.log("License does not exist");
		isActivated = false;
		askForToken();
		return;
	}
	logger.log("License file exists");
	std::string hashcode;
	getline(hashfile, hashcode);

	if(!hashcode.length()){
		//Hash Code does not exist
		logger.log("Hash Code does not exist");
		isActivated = false;
		askForToken();
		return;
	}
	logger.log("Hash Code exists");
	try {
		conn->setSchema("labassistor");

		sql::Statement *stmt = conn->createStatement();
		sql::ResultSet *res = stmt->executeQuery("SELECT token FROM admin");
		res->next();
		std::string token = res->getString("token");

		if (token.length() != 19) {
			logger.log("Token Error From MYSQL");
			askForToken();
			return;
		}
		logger.log("Token Exists with matching size");
		logger.log(token);
		//********************
		char* cipher = new char[256];
		cipher = encrypt(token.c_str());//Encrypt token
		if (!cipher) {
			logger.log("Invalid Token");
			delete[] cipher;
			askForToken();
			return;
		}
		std::string licensecipher(cipher);//Chekc for match

		if (licensecipher.length() != hashcode.length()) {
			logger.log("Token hash length does not match");
			logger.log("License Len: " + std::to_string(licensecipher.length()));
			logger.log("Hashcode Len: " + std::to_string(hashcode.length()));
			delete[] cipher;
			askForToken();
			return;
		}

		int isnotEqual = std::memcmp(licensecipher.c_str(), hashcode.c_str(), licensecipher.length());
		if (isnotEqual) {
			logger.log("Token secret does not match");
			delete[] cipher;
			askForToken();
		}

		delete[] cipher;
		return;
	}
	catch (exception e) {
		logger.log("Exception occured while ActivationCheck");
		logger.log(e.what());
		isActivated = false;
		askForToken();
		return;
	}
}


bool SignIn::checkEmailFormat(std::string email) {
	std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	if (std::regex_match(email, pattern)) {
		return true;
	}
	else {
		return false;
	}
}

bool SignIn::checkDetails(std::string email, std::string password) {
	logger->log("Checking details...");
	try {
		conn->setSchema("labassistor");
		sql::Statement* stmt = conn->createStatement();
		sql::SQLString query("SELECT email, password FROM admin where email = " + QUOTE + email + QUOTE + " and password = " + QUOTE + password + QUOTE);
		sql::ResultSet* res = stmt->executeQuery(query);
		if (res->rowsCount() > 0) {
			isLoggedin = true;
			logger->log("User logged in");
			RECT rc; // Specify the rectangle to invalidate
			GetClientRect(hMain, &rc); // Invalidate the entire client area
			InvalidateRect(hMain, &rc, TRUE); // TRUE indicates that the entire rectangle should be erased before repainting
			Destroy();
			return true;
		}
		logger->log("User login failed");
		return false;
	}
	catch (std::exception e) {
		logger->log("Error while logging in user");
	}
}

bool SignIn::getStatus() const{
	return isLoggedin;
}

void SignIn::Destroy() {
	DestroyWindow(hMain);
}