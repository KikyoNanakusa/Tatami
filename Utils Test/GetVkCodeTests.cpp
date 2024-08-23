#include "pch.h"
#include "gtest/gtest.h"
#include "KeyUtils.h"

// 大文字のテスト
TEST(GetVkCodeTests, UpperCaseLetters) {
    EXPECT_EQ(0x48, getVkCodeFromChar('H'));  
    EXPECT_EQ(0x41, getVkCodeFromChar('A'));  
}

// 小文字のテスト
TEST(GetVkCodeTests, LowerCaseLetters) {
    EXPECT_EQ(0x48, getVkCodeFromChar('h'));  
    EXPECT_EQ(0x41, getVkCodeFromChar('a')); 
}

// 数字のテスト
TEST(GetVkCodeTests, Digits) {
    EXPECT_EQ(0x30, getVkCodeFromChar('0'));
    EXPECT_EQ(0x39, getVkCodeFromChar('9'));
}

// 特殊文字のテスト
TEST(GetVkCodeTests, SpecialCharacters) {
    EXPECT_EQ(0xBA, getVkCodeFromChar(';'));
    EXPECT_EQ(0xDE, getVkCodeFromChar('\'')); 
}

// 無効な文字のテスト
TEST(GetVkCodeTests, InvalidCharacter) {
    EXPECT_EQ(-1, getVkCodeFromChar('\0'));  
}
