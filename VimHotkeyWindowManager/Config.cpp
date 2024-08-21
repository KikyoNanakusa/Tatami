#include "Config.h"

class Config {
public:
    bool loadConfig(const std::string& configString) {
        return true;
    }

    std::string getConfigValue(const std::string& key) {
		return "";
	}

private:
    std::map<std::string, std::string> configMap;

    bool parseConfigString(const std::string& configString) {
		return true;
	}

};