#include "Config.h"

class Config {
public:
    Config() {
        configMap = std::map<int, int>();
    }

    bool loadConfig(const std::string& configString) {
        return true;
    }

    std::map<int, int> getConfigMap(const int) {
        return {};
	}

private:
    std::map<int, int> configMap;

    bool parseConfigString(const std::string& configString) {
		return true;
	}

};