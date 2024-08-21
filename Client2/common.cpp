#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <windows.h>
#include <iphlpapi.h>
#include <string>
#include <timeapi.h>
#include <locale>
#include <codecvt>


#pragma comment(lib, "IPHLPAPI.lib")

#include "common.h"
#include "Logger.h"


void readConfig(std::string filename, std::string* server, std::string* port) {
    FileLogger logger("common_CPP.txt");
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


bool isVirtualAdapter(const std::string& description) {
    // Add more conditions as needed to filter out virtual adapters
    return (description.find("Virtual") != std::string::npos) ||
        (description.find("VMware") != std::string::npos);
}

bool isEthernetAdapter(const IP_ADAPTER_INFO* adapterInfo) {
    return adapterInfo->Type == MIB_IF_TYPE_ETHERNET;
}

std::string getEthernetIPAddresses() {
    FileLogger logger("log.txt");
    std::string result;

    // Buffer to store adapter information
    IP_ADAPTER_INFO* adapterInfo = nullptr;
    ULONG bufferSize = 0;

    // First, call GetAdaptersInfo to get the required buffer size
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
        adapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(malloc(bufferSize));

        // Call GetAdaptersInfo again to get the actual information
        if (GetAdaptersInfo(adapterInfo, &bufferSize) == NO_ERROR) {
            // Iterate through the list of adapters
            IP_ADAPTER_INFO* currentAdapter = adapterInfo;
            while (currentAdapter != nullptr) {
                // Check if the adapter is not a virtual one and is an Ethernet adapter
                if (!isVirtualAdapter(currentAdapter->Description) &&
                    isEthernetAdapter(currentAdapter)) {
                    // Iterate through the list of IP addresses for the current adapter
                    IP_ADDR_STRING* ipAddress = &currentAdapter->IpAddressList;
                    while (ipAddress != nullptr) {
                        result += ipAddress->IpAddress.String;
                        result += " ";
                        ipAddress = ipAddress->Next;
                    }
                }

                currentAdapter = currentAdapter->Next;
            }
        }
        else {
            logger.log("Error getting adapter information (second call to GetAdaptersInfo)");
        }

        // Free allocated memory
        free(adapterInfo);
    }
    else {
        logger.log("Error getting adapter information (first call to GetAdaptersInfo)");
    }

    return result;
}


std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

std::string getComputerName() {
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);

    if (GetComputerName(computerName, &size)) {
        return converter.to_bytes(computerName);
    }
    else {
        return "Unknown";
    }
}

std::string getSystemType() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        return "x64";
    }
    else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        return "x86";
    }
    else {
        return "Unknown";
    }
}

std::string getProcessor() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    switch (systemInfo.dwProcessorType) {
    case PROCESSOR_INTEL_PENTIUM:
        return "Intel Pentium";
    case PROCESSOR_INTEL_IA64:
        return "Intel Itanium";
    case PROCESSOR_AMD_X8664:
        return "AMD64";
    default:
        return "Unknown";
    }
}

std::string getTimeZone() {
    TIME_ZONE_INFORMATION timeZoneInfo;
    GetTimeZoneInformation(&timeZoneInfo);

    return converter.to_bytes(timeZoneInfo.StandardName);
}

std::string getSystemManufacturer() {
    return "Gigabyte";  // Replace with actual method to get manufacturer info
}

std::string getOSManufacturer() {
    return "Microsoft";  // Replace with actual method to get OS manufacturer info
}


std::string getSystemInfoString() {
    std::stringstream ss;
    ss << "{SystemName:" << getComputerName().c_str()
        << ", SystemType:" << getSystemType().c_str()
        << ", Processor:" << getProcessor().c_str()
        << ", TimeZone:" << getTimeZone().c_str()
        << ", SystemManufacturer:" << getSystemManufacturer().c_str()
        << ", OSManufacturer:" << getOSManufacturer().c_str()
        << ", IP:" << getEthernetIPAddresses().c_str()
        << "}";

    return ss.str();
}
