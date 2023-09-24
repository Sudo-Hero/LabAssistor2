#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "common.h"
#include "Logger.h"

void readConfig(std::string filename, std::string* server, std::string* port) {
    FileLogger logger("log.txt");
    // Define a map to store configuration key-value pairs
    std::map<std::string, std::string> config;

    // Open the configuration file
    std::ifstream configFile(filename);

    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            // Split each line into key and value using '=' as the delimiter
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(std::getline(iss, key, '='), value)) {
                config[key] = value;
            }
        }
        *server = config["server"];
        *port = config["port"];
        // Close the configuration file
        configFile.close();
    }
    else {
        logger.log("Failed to open the configuration file.");
    }

}
