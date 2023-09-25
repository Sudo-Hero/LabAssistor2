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

void readConfigMYSQL(std::string filename, std::string* server, std::string* port, std::string* username, std::string* password) {
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
        *username = config["username"];
        *password = config["password"];
        // Close the configuration file
        configFile.close();
    }
    else {
        logger.log("Failed to open the configuration file.");
    }

}

std::string removeTrailingSpaces(const std::string& input) {
    // Find the position of the first non-space character from the end
    size_t endPos = input.find_last_not_of(" ");
    
    if (endPos != std::string::npos) {
        // Use substr to extract the part of the string without trailing spaces
        return input.substr(0, endPos + 1);
    } else {
        // The input string consists only of spaces
        return "";
    }
}