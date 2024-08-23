#include "gtest/gtest.h"
#include "pch.h"
#include "Utils.h"
#include "Utils.cpp"

TEST(TrimTests, TrimsLeadingWhitespace) {
    EXPECT_EQ("Hello", trim("   Hello"));
    EXPECT_EQ("World", trim("\t\tWorld"));
    EXPECT_EQ("Test", trim("\nTest"));
}

TEST(TrimTests, TrimsTrailingWhitespace) {
    EXPECT_EQ("Hello", trim("Hello   "));
    EXPECT_EQ("World", trim("World\t\t"));
    EXPECT_EQ("Test", trim("Test\n"));
}

TEST(TrimTests, TrimsBothLeadingAndTrailingWhitespace) {
    EXPECT_EQ("Hello", trim("   Hello   "));
    EXPECT_EQ("World", trim("\t\tWorld\t\t"));
    EXPECT_EQ("Test", trim("\nTest\n"));
}

TEST(TrimTests, NoWhitespace) {
    EXPECT_EQ("Hello", trim("Hello"));
    EXPECT_EQ("World", trim("World"));
    EXPECT_EQ("Test", trim("Test"));
}

TEST(TrimTests, EmptyString) {
    EXPECT_EQ("", trim(""));
}

TEST(TrimTests, OnlyWhitespace) {
    EXPECT_EQ("", trim("   "));
    EXPECT_EQ("", trim("\t\t"));
    EXPECT_EQ("", trim("\n"));
}
