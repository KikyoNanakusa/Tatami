#include "Config.h"

Config::Config() {
	configMap = std::map<int, int>();
}

bool Config::loadConfig(const std::string& configString) {
	return true;
}

std::map<int, int> Config::getConfigMap(const int) {
	return {};
}


bool Config::parseConfigString(const std::string& configString) {
	return true;
}
