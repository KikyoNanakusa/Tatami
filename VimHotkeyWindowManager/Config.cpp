#include "Config.h"

Config::Config() {
	configMap = std::map<Action, SHORT>();
}

bool Config::loadConfig(const std::string& configString) {
	if (!parseConfigString(configString)) {
		return false;
	}
	return true;
}

std::map<Action, SHORT> Config::getConfigMap() {
	return configMap;
}

SHORT Config::getConfigValue(Action action) {
	return configMap[action];
}

bool Config::parseConfigString(const std::string& configString) {
    std::istringstream configStream(configString);
    std::string line;

    while (std::getline(configStream, line)) {
        if (!parseLine(line)) {
            return false;
        }
    }
    return true;
}

bool Config::parseLine(const std::string& line) {
    std::istringstream lineStream(line);
    std::string key, value;

    // Check if the line is empty or a comment
    if (isWhitespaceOnly(line) || trim(line)[0] == '#') {
		return true;
	}


    if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
        key = trim(key);
        value = trim(value);

        // キーが2文字以上の場合は無効
        if (value.size() != 1) {
            std::cerr << "Invalid value size for key: " << key << std::endl;
            return false;
        }
        return setConfigValue(key, value[0]);
    }

    std::cerr << "Failed to parse line: " << line << std::endl;
    return false;
}

bool Config::setConfigValue(const std::string& key, char value) {
    SHORT vkCode = getVkCodeFromChar(value);

    if (key == "MoveWindowLeft") {
        configMap[ACTION_MOVE_WINDOW_LEFT] = vkCode;
    } else if (key == "MoveWindowDown") {
        configMap[ACTION_MOVE_WINDOW_DOWN] = vkCode;
    } else if (key == "MoveWindowUp") {
        configMap[ACTION_MOVE_WINDOW_UP] = vkCode;
    } else if (key == "MoveWindowRight") {
        configMap[ACTION_MOVE_WINDOW_RIGHT] = vkCode;
    } else if (key == "MinimizeWindow") {
        configMap[ACTION_WINDOW_MINIMIZE] = vkCode;
    } else if (key == "RestoreWindow") {
        configMap[ACTION_WINDOW_RESTORE] = vkCode;
    } else if (key == "MaximizeWindow") {
        configMap[ACTION_WINDOW_MAXIMIZE] = vkCode;
    } else if (key == "MoveFocusLeft") {
		configMap[ACTION_MOVE_FOCUS_LEFT] = vkCode;
	} else if (key == "MoveFocusDown") {
		configMap[ACTION_MOVE_FOCUS_DOWN] = vkCode;
	} else if (key == "MoveFocusUp") {
		configMap[ACTION_MOVE_FOCUS_UP] = vkCode;
	} else if (key == "MoveFocusRight") {
		configMap[ACTION_MOVE_FOCUS_RIGHT] = vkCode;
    } else {
        std::cerr << "Unknown key: " << key << std::endl;
        return false;
    }

    return true;
}
