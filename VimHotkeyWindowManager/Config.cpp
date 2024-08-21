#include "Config.h"

Config::Config() {
	configMap = std::map<int, int>();
}

bool Config::loadConfig(const std::string& configString) {
	if (!parseConfigString(configString)) {
		return false;
	}
	return true;
}

std::map<int, int> Config::getConfigMap(const int) {
	return configMap;
}

bool Config::parseConfigString(const std::string& configString) {
	std::istringstream configStream(configString);
	std::string line;

	while (std::getline(configStream, line)) {
		std::istringstream lineStream(line);
		std::string key, value;
		if (std::getline(lineStream, key, '=')) {
			if (std::getline(lineStream, value)) {

				// ƒL[‚ª2•¶ŽšˆÈã‚Ìê‡‚Í–³Œø
				if (value.size() >= 2) {
					return false;
				} 

				if (key == "MoveWindowLeft") {
					configMap[ACTION_MOVE_WINDOW_LEFT] = static_cast<int>(value[0]);
				}
				else if (key == "MoveWindowDown") {
					configMap[ACTION_MOVE_WINDOW_DOWN] = static_cast<int>(value[0]);
				}
				else if (key == "MoveWindowUp") {
					configMap[ACTION_MOVE_WINDOW_UP] = static_cast<int>(value[0]);
				}
				else if (key == "MoveWindowRight") {
					configMap[ACTION_MOVE_WINDOW_RIGHT] = static_cast<int>(value[0]);
				}
				else if (key == "MinimizeWindow") {
					configMap[ACTION_WINDOW_MINIMIZE] = static_cast<int>(value[0]);
				}
				else if (key == "RestoreWindow") {
					configMap[ACTION_WINDOW_RESTORE] = static_cast<int>(value[0]);
				}
				else if (key == "MaximizeWindow") {
					configMap[ACTION_WINDOW_MAXIMIZE] = static_cast<int>(value[0]);
				}
				else {
					std::cerr << "Unknown key: " << key << std::endl;
					return false;
				}
			}
		}
	}
	return true;
}
