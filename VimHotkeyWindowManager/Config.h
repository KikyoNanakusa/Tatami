#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "definition.h"
#include "KeyUtils.h"
#include "Utils.h"

class Config {
public:
	Config();
	bool loadConfig(const std::string& configString);
	std::map<Action, SHORT> getConfigMap();
	SHORT getConfigValue(Action action);
private:
	std::map<Action, SHORT> configMap;
	bool parseConfigString(const std::string& configString);
	bool parseLine(const std::string& line);
	bool setConfigValue(const std::string& key, char value);
};
