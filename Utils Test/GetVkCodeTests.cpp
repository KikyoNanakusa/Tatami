#include "pch.h"
#include "gtest/gtest.h"
#include "KeyUtils.h"

// �啶���̃e�X�g
TEST(GetVkCodeTests, UpperCaseLetters) {
    EXPECT_EQ(0x48, getVkCodeFromChar('H'));  
    EXPECT_EQ(0x41, getVkCodeFromChar('A'));  
}

// �������̃e�X�g
TEST(GetVkCodeTests, LowerCaseLetters) {
    EXPECT_EQ(0x48, getVkCodeFromChar('h'));  
    EXPECT_EQ(0x41, getVkCodeFromChar('a')); 
}

// �����̃e�X�g
TEST(GetVkCodeTests, Digits) {
    EXPECT_EQ(0x30, getVkCodeFromChar('0'));
    EXPECT_EQ(0x39, getVkCodeFromChar('9'));
}

// ���ꕶ���̃e�X�g
TEST(GetVkCodeTests, SpecialCharacters) {
    EXPECT_EQ(0xBA, getVkCodeFromChar(';'));
    EXPECT_EQ(0xDE, getVkCodeFromChar('\'')); 
}

// �����ȕ����̃e�X�g
TEST(GetVkCodeTests, InvalidCharacter) {
    EXPECT_EQ(-1, getVkCodeFromChar('\0'));  
}
