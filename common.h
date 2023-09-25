#pragma once
#ifndef _COMMON_
#define _COMMON_

#include <iostream>

void readConfig(std::string filename, std::string* server, std::string* port);
void readConfigMYSQL(std::string filename, std::string* server, std::string* port, std::string* username, std::string* password);

std::string removeTrailingSpaces(const std::string& input);

#endif // !_COMMON_
