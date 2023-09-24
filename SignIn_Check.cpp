#include "mysql_conn.h"
#include "SignIn_Check.h"
#include <fstream>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>
#include "Logger.h"
#include <windows.h>
#include <sha256.h>

void SignIn::askForToken() {
	EnableWindow(hMain, false);
	activationbox.RegisterActClass();
	activationbox.CreateAcivationWindow(hMain);
}

void SignIn::ActivationCheck() {
	FileLogger logger("log.txt");
	std::ifstream hashfile("license.txt", std::ios::in);
	
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
	if (hashcode.length() != 64) {
		//Hash Code length mismatch
		logger.log("Hash Code length mismatch");
		askForToken();
		return;
	}
	logger.log("Hash Code length matched");
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
		SHA256 sha256;
		std::string tokenhash = sha256(token);
		logger.log(tokenhash);
		logger.log(std::to_string(tokenhash.length()));
		if (tokenhash.length() != 64) {
			logger.log("Hash Code Failed");
			askForToken();
			return;
		}
		if (tokenhash != hashcode) {
			logger.log("Token hash Does not match with license hash");
			askForToken();
			return;
		}
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
