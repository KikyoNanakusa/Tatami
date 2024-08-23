#include "pch.h"
#include "gtest/gtest.h"
#include "ConfigReader.h"
#include "ConfigReader.cpp"
#include <string>
#include <fstream>

std::string ReadFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string NormalizeNewlines(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (c != '\r') {
            result += c;
        }
    }
    return result;
}

const std::string APP_NAME = "VWindowManager";
const std::string CONFIG_FILE_NAME = "\\config.txt";
TEST(ConfigReaderTests, CreateDirectoryAndFileIfNotExist) {
    std::string appDataPath = GetAppDataPath(APP_NAME);
    std::string configPath = appDataPath + CONFIG_FILE_NAME;

    if (isPathExists(configPath)) {
        remove(configPath.c_str());
    }
    if (isPathExists(appDataPath)) {
        RemoveDirectoryA(appDataPath.c_str());
    }

    std::string content = ReadConfigFile();
    EXPECT_FALSE(content.empty());

    EXPECT_TRUE(isPathExists(configPath));

    std::string fileContent = ReadFile(configPath);
    EXPECT_EQ(fileContent, DefaultConfig);

    // Clean up
    remove(configPath.c_str());
    RemoveDirectoryA(appDataPath.c_str());
}

TEST(ConfigReaderTests, ReadExistingConfigFile) {
    std::string appDataPath = GetAppDataPath(APP_NAME);
    std::string configPath = appDataPath + CONFIG_FILE_NAME;

    if (!isPathExists(appDataPath)) {
		CreateDirectoryA(appDataPath.c_str(), NULL);
	}

    std::ofstream outFile(configPath);
    std::string testContent = "MoveWindowLeft=H\nMoveWindowDown=J\n";
    outFile << testContent;
    outFile.close();

    std::string content = ReadConfigFile();
    EXPECT_EQ(NormalizeNewlines(content), testContent);

    // Clean up
    remove(configPath.c_str());
    RemoveDirectoryA(appDataPath.c_str());
}

TEST(ConfigReaderTests, ConfigFileNotRecreatedIfExists) {
    std::string appDataPath = GetAppDataPath(APP_NAME);
    std::string configPath = appDataPath + CONFIG_FILE_NAME;

    if (!isPathExists(appDataPath)) {
		CreateDirectoryA(appDataPath.c_str(), NULL);
	}

    std::ofstream outFile(configPath);
    std::string testContent = "MoveWindowLeft=H\n";
    outFile << testContent;
    outFile.close();

    std::string content = ReadConfigFile();
    EXPECT_EQ(NormalizeNewlines(content), testContent);

    std::string fileContent = ReadFile(configPath);
    EXPECT_EQ(fileContent, testContent);

    // Clean up
    remove(configPath.c_str());
    RemoveDirectoryA(appDataPath.c_str());
}

