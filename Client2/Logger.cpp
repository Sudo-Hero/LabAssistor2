#include "Logger.h"

void FileLogger::log(const std::string& message) {
    if (logFile.is_open()) {
        std::time_t now;
        time(&now);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &now); // Use ctime_s for a safer timestamp
        timestamp[strlen(timestamp) - 1] = '\0'; // Remove the newline character

        logFile << "[" << timestamp << "] " << message << std::endl;
    }
}