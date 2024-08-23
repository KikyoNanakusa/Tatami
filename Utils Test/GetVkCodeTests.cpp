#include "pch.h"
#include "gtest/gtest.h"
#include "KeyUtils.h"
#include "KeyUtils.cpp"

// 大文字のテスト
TEST(GetVkCodeTests, UpperCaseLetters) {
    EXPECT_EQ(0x48, getVkCodeFromChar('H'));  
    EXPECT_EQ(0x4A, getVkCodeFromChar('J'));
    EXPECT_EQ(0x4B, getVkCodeFromChar('K'));
    EXPECT_EQ(0x4C, getVkCodeFromChar('L'));
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

