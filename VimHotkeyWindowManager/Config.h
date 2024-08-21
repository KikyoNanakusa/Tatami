#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "definition.h"

class Config {
public:
	Config();
	bool loadConfig(const std::string& configString);
	std::map<int, int> getConfigMap(const int);
private:
	std::map<int, int> configMap;
	bool parseConfigString(const std::string& configString);
};
