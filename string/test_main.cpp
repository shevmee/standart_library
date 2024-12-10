#include <gtest/gtest.h>
#include "String.hpp"

using String = BasicString<char>;

TEST(BasicStringTest, DefaultConstructor) {
    String str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.c_str(), nullptr);
}

TEST(BasicStringTest, CopyConstructor) {
    String str1("Hello");
    String str2(str1);
    EXPECT_EQ(str2.size(), 5);
    EXPECT_FALSE(str2.empty());
    EXPECT_STREQ(str2.c_str(), "Hello");
    EXPECT_EQ(std::strcmp(str2.c_str(), "Hello"), 0);
}

TEST(BasicStringTest, SubstringConstructor) {
    String str1("Hello, World!");
    String substr(str1, 7, 5);
    
    EXPECT_EQ(substr.size(), 5);
    EXPECT_EQ(std::strcmp(substr.c_str(), "World"), 0);
}

TEST(BasicStringTest, SubstringConstructorWithOutOfRangePos) {
    String str1("Hello, World!");
    EXPECT_THROW({
        String substr(str1, 20, 5);
    }, std::out_of_range);
}

TEST(BasicStringTest, CStringConstructor) {
    String str("Hello");
    EXPECT_EQ(str.size(), 5);
    EXPECT_FALSE(str.empty());
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST(BasicStringTest, FillConstructor) {
    String str(5, 'A');
    EXPECT_EQ(str.size(), 5);
    EXPECT_EQ(std::strcmp(str.c_str(), "AAAAA"), 0);
}

TEST(BasicStringTest, CopyAssignmentOperator) {
    String str1("Hello");
    String str2;
    str2 = str1;
    
    EXPECT_EQ(str2.size(), 5);
    EXPECT_EQ(std::strcmp(str2.c_str(), "Hello"), 0);
    
    str2 = str2;
    EXPECT_EQ(str2.size(), 5);
    EXPECT_EQ(std::strcmp(str2.c_str(), "Hello"), 0);
}

TEST(BasicStringTest, Destructor) {
    String str("Hello");
    EXPECT_NO_THROW({
        String str2("Hello");
    });
}

TEST(BasicStringTest, CStrMethod) {
    String str("Hello");
    EXPECT_EQ(std::strcmp(str.c_str(), "Hello"), 0);
}

TEST(BasicStringTest, SizeMethod) {
    String str("Hello");
    EXPECT_EQ(str.size(), 5);
}

TEST(BasicStringTest, LengthMethod) {
    String str("Hello");
    EXPECT_EQ(str.length(), 5);
}

TEST(BasicStringTest, CapacityMethod) {
    String str("Hello");
    EXPECT_GE(str.capacity(), 6);
}

TEST(BasicStringTest, EmptyMethod) {
    String str1("Hello");
    String str2;
    EXPECT_FALSE(str1.empty());
    EXPECT_TRUE(str2.empty());
}

TEST(BasicStringTest, ClearMethod) {
    String str("Hello");
    str.clear();
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0);
}

TEST(BasicStringTest, FindMethod) {
    String str("Hello, dear Santa!");
    String substr("dear");
    auto res = str.find(substr, 0);
    EXPECT_EQ(res, 7);
}

TEST(BasicStringTest, CompareMethod) {
    String str1("Hello, dear Santa!");
    String str2("Hello, dear Santa Clause!");
    auto res = str1.compare(str2);
    EXPECT_EQ(res, 1);
}

TEST(BasicStringTests, OutputStreamOperator) {
    BasicString<char> str("Hello, Stream!");
    
    std::ostringstream os;
    os << str;
    
    EXPECT_EQ(os.str(), "Hello, Stream!");
}

TEST(BasicStringTests, InputStreamOperator) {
    std::istringstream input("Hello from Input Stream");
    BasicString<char> str;
    
    input >> str;
    
    EXPECT_STREQ(str.c_str(), "Hello from Input Stream");
}

TEST(BasicStringTests, InputWithSpaces) {
    std::istringstream input("Hello World 123");
    BasicString<char> str;
    
    input >> str;
    
    EXPECT_STREQ(str.c_str(), "Hello World 123");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}