#ifndef _LOGGER_
#define _LOGGER_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>





class FileLogger {
public:
    FileLogger(const std::string& filename) {
        logFile.open(filename, std::ios::out | std::ios::app); // Open the file in append mode
        if (!logFile.is_open()) {
            std::cerr << "Error: Could not open log file." << std::endl;
        }
    }

    ~FileLogger() {
        if (logFile.is_open()) {
            logFile.close(); // Close the log file when the logger is destroyed
        }
    }

    void log(const std::string& message);

private:
    std::ofstream logFile;
};
#endif // !_LOGGER_
/**
int main() {
    FileLogger logger("log.txt"); // Create a logger and specify the log file name

    // Log some messages
    logger.log("This is a log message.");
    logger.log("Another log message.");

    // The log file will be automatically closed when the logger goes out of scope

    return 0;
}

**/