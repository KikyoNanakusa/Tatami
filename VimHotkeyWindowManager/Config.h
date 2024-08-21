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
	std::map<Action, int> getConfigMap();
private:
	std::map<Action, int> configMap;
	bool parseConfigString(const std::string& configString);
};
