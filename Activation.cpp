#include "common.h"
#include "Logger.h"

bool submitKey() {
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

	}
	catch(...) {
		logger.log("Exception in activation!!");
		return false;
	}
}