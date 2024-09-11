#include "pch.h"
#include "gtest/gtest.h"
#include "Config.h"
#include "Config.cpp"


TEST(ConfigTests, LoadValidConfig) {
    Config config;
    std::string configString = 
        "MoveWindowLeft=H\n"
        "MoveWindowDown=J\n"
        "MoveWindowUp=K\n"
        "MoveWindowRight=L\n"
        "MinimizeWindow=D\n"
        "RestoreWindow=U\n"
        "MaximizeWindow=G\n";

    bool result = config.loadConfig(configString);
    
    ASSERT_TRUE(result);

    auto configMap = config.getConfigMap();
    EXPECT_EQ(0x48, configMap[ACTION_MOVE_WINDOW_LEFT]);  
    EXPECT_EQ(0x4A, configMap[ACTION_MOVE_WINDOW_DOWN]);  
    EXPECT_EQ(0x4B, configMap[ACTION_MOVE_WINDOW_UP]);    
    EXPECT_EQ(0x4C, configMap[ACTION_MOVE_WINDOW_RIGHT]); 
    EXPECT_EQ(0x44, configMap[ACTION_WINDOW_MINIMIZE]);   
    EXPECT_EQ(0x55, configMap[ACTION_WINDOW_RESTORE]);    
    EXPECT_EQ(0x47, configMap[ACTION_WINDOW_MAXIMIZE]);   
}

TEST(ConfigTests, LoadComment) {
	Config config;
	std::string configString = 
		"# This is a comment\n"
		"MoveWindowLeft=H\n"
		"   # This is another comment\n"
        "       # tab\n"
        "\n"
        "    \n"
		"MoveWindowDown=J\n";

	bool result = config.loadConfig(configString);
	
	ASSERT_TRUE(result);

	auto configMap = config.getConfigMap();
	EXPECT_EQ(0x48, configMap[ACTION_MOVE_WINDOW_LEFT]);  
	EXPECT_EQ(0x4A, configMap[ACTION_MOVE_WINDOW_DOWN]);  
}
TEST(ConfigTests, LoadInvalidConfig_MultipleCharactersInValue) {
    Config config;
    std::string configString = 
        "MoveWindowLeft=HH\n"  // 無効: 値が2文字
        "MoveWindowDown=J\n";

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  // エラーが発生することを期待
}

TEST(ConfigTests, LoadInvalidConfig_UnknownKey) {
    Config config;
    std::string configString = 
        "UnknownKey=X\n"
        "MoveWindowDown=J\n";

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  // エラーが発生することを期待
}

TEST(ConfigTests, LoadInvalidConfig_EmptyValue) {
    Config config;
    std::string configString = 
        "MoveWindowLeft=\n";  // 無効: 値が空

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  // エラーが発生することを期待
}

TEST(ConfigTests, LoadEmptyConfig) {
    Config config;
    std::string configString = "";

    bool result = config.loadConfig(configString);
    
    ASSERT_TRUE(result);  // 空のコンフィグはエラーなく処理されることを期待
    auto configMap = config.getConfigMap();
    EXPECT_TRUE(configMap.empty());  // マップが空であることを期待
}

TEST(ConfigTests, LoadInvalidConfig_MultipleUnknownKeys) {
    Config config;
    std::string configString = 
        "MoveWindowLeft=H\n"
        "MoveWindowDown=J\n"
        "InvalidKey1=X\n"    
        "InvalidKey2=Y\n";   

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  
}

TEST(ConfigTests, LoadInvalidConfig_ValidAndInvalidKeys) {
    Config config;
    std::string configString = 
        "MoveWindowLeft=H\n"
        "MoveWindowDown=J\n"
        "InvalidKey=Z\n"   // 想定外のキー
        "MoveWindowUp=K\n";

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  
}

TEST(ConfigTests, LoadInvalidConfig_OnlyUnknownKeys) {
    Config config;
    std::string configString = 
        "UnknownKey1=A\n"  // 想定外のキー
        "UnknownKey2=B\n"  // 想定外のキー
        "UnknownKey3=C\n"; // 想定外のキー

    bool result = config.loadConfig(configString);
    
    ASSERT_FALSE(result);  
}
