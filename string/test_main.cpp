#include <gtest/gtest.h>
#include <string>
#include <cstring>
#include "BasicString.hpp"

class BasicStringTest : public ::testing::Test {
protected:
    BasicString<char> str1;
    BasicString<char> str2;
    BasicString<char> str3;
    BasicString<char> str4;

    void SetUp() override {
        str1 = "Hello";
        str2 = "World";
        str3 = "BasicString";
        str4 = "AnotherTestString";
    }
};

TEST_F(BasicStringTest, DefaultConstructor) {
    BasicString<char> empty_str;
    EXPECT_EQ(empty_str.size(), 0);
    EXPECT_EQ(empty_str.capacity(), 0);
    EXPECT_TRUE(empty_str.empty());
}

TEST_F(BasicStringTest, CStrConstructor) {
    BasicString<char> str("Hello, World!");
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.c_str(), "Hello, World!");
}

TEST_F(BasicStringTest, CopyConstructor) {
    BasicString<char> copied_str(str1);
    EXPECT_EQ(copied_str.size(), str1.size());
    EXPECT_STREQ(copied_str.c_str(), "Hello");
}

TEST_F(BasicStringTest, SubstringConstructor) {
    BasicString<char> sub_str(str3, 0, 5);
    EXPECT_EQ(sub_str.size(), 5);
    EXPECT_STREQ(sub_str.c_str(), "Basic");
}

TEST_F(BasicStringTest, nCharactersConstructor) {
    BasicString<char> sub_str(10, 'c');
    EXPECT_EQ(sub_str.size(), 10);
    EXPECT_STREQ(sub_str.c_str(), "cccccccccc");
}

TEST_F(BasicStringTest, OperatorAssignment) {
    BasicString<char> new_str;
    new_str = str2;
    EXPECT_EQ(new_str.size(), str2.size());
    EXPECT_STREQ(new_str.c_str(), "World");
}

TEST_F(BasicStringTest, SelfAssignment) {
    BasicString<char> original = str1;
    original = original;
    EXPECT_EQ(original.size(), str1.size());
    EXPECT_STREQ(original.c_str(), "Hello");
}

TEST_F(BasicStringTest, OperatorSquaredBrackets) {
    auto i_char = str3[8];
    str3[8] = 'L';
    std::cout << str3 << std::endl;
    EXPECT_STREQ(str3.c_str(), "BasicStrLng");
}

TEST_F(BasicStringTest, DataAccess) {
    auto ptr1 = str1.c_str();
    auto ptr2 = str1.data();
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(BasicStringTest, CastingOperatorToStringView) {
    std::basic_string_view<char> view1 = str1;
    std::basic_string_view<char> view2 = str2;
    std::basic_string_view<char> view3 = str3;
    std::basic_string_view<char> view4 = str4;

    EXPECT_EQ(view1, "Hello");
    EXPECT_EQ(view1.size(), 5);

    EXPECT_EQ(view2, "World");
    EXPECT_EQ(view2.size(), 5);

    EXPECT_EQ(view3, "BasicString");
    EXPECT_EQ(view3.size(), 11);

    EXPECT_EQ(view4, "AnotherTestString");
    EXPECT_EQ(view4.size(), 17);

    EXPECT_EQ(view1.data(), str1.data());
    EXPECT_EQ(view2.data(), str2.data());
    EXPECT_EQ(view3.data(), str3.data());
    EXPECT_EQ(view4.data(), str4.data());
}

TEST_F(BasicStringTest, EmptyStringView) {
    BasicString<char> str("");
    std::basic_string_view<char> view = str;
    EXPECT_EQ(view.size(), 0);
    EXPECT_EQ(view.data(), str.data());
}

TEST_F(BasicStringTest, NonEmptyStringView) {
    std::basic_string_view<char> view = str3;
    EXPECT_EQ(view.size(), 11);
    EXPECT_EQ(view.data(), str3.data());
}

TEST_F(BasicStringTest, Capacity) {
    EXPECT_EQ(str1.size(), 5);
    EXPECT_EQ(str2.length(), 5);
    EXPECT_EQ(str3.capacity(), 12);
    EXPECT_TRUE(BasicString<char>().empty());
}

TEST_F(BasicStringTest, ReserveLargerCapacity) {
    size_t initial_capacity = str2.capacity();
    str2.reserve(50);
    
    EXPECT_GT(str2.capacity(), initial_capacity);
    EXPECT_EQ(str2.size(), 5);
}

TEST_F(BasicStringTest, ReserveLargeCapacity) {
    str1.reserve(1000000);
    EXPECT_GE(str1.capacity(), 1000000);
    EXPECT_EQ(str1.size(), 5);
}

TEST_F(BasicStringTest, ReserveZeroCapacity) {
    str1.reserve(0);
    EXPECT_GE(str1.capacity(), 5);
}

TEST_F(BasicStringTest, ReserveSmallerCapacity) {
    size_t initial_capacity = str2.capacity();
    str2.reserve(4);
    EXPECT_EQ(str2.capacity(), initial_capacity);
}

TEST_F(BasicStringTest, ReserveSameCapacity) {
    size_t initial_capacity = str2.capacity();
    str2.reserve(initial_capacity);
    
    EXPECT_EQ(str2.capacity(), initial_capacity);
}

TEST_F(BasicStringTest, ReserveEdgeCase) {
    str3.reserve(5);
    EXPECT_EQ(str3.capacity(), 12);
    EXPECT_EQ(str3.size(), 11);
}

TEST_F(BasicStringTest, ReserveForEmptyString) {
    BasicString<char> empty_str;
    size_t initial_capacity = empty_str.capacity();
    empty_str.reserve(10);
    EXPECT_EQ(empty_str.capacity(), 10);
    EXPECT_EQ(empty_str.size(), 0);
}

TEST_F(BasicStringTest, PushBack) {
    str1.push_back('!');
    EXPECT_EQ(str1.size(), 6);
    EXPECT_STREQ(str1.c_str(), "Hello!");
}

TEST_F(BasicStringTest, PopBack) {
    str1.pop_back();
    EXPECT_EQ(str1.size(), 4);
    EXPECT_STREQ(str1.c_str(), "Hell");
}

TEST_F(BasicStringTest, Replace) {
    str1.replace(0, 4, "Hi");
    EXPECT_EQ(str1.size(), 3);
    EXPECT_STREQ(str1.c_str(), "Hio");
}

TEST_F(BasicStringTest, PushBackAndResize) {
    str1.push_back('!');
    EXPECT_EQ(str1.size(), 6);
    EXPECT_STREQ(str1.c_str(), "Hello!");

    str1.resize(3, 'X');
    EXPECT_EQ(str1.size(), 3);
    EXPECT_STREQ(str1.c_str(), "Hel");
}

TEST_F(BasicStringTest, Resize) {
    str1.resize(10, 'X');
    EXPECT_EQ(str1.size(), 10);
    EXPECT_STREQ(str1.c_str(), "HelloXXXXX");
}

TEST_F(BasicStringTest, Clear) {
    str1.clear();
    EXPECT_EQ(str1.size(), 0);
    EXPECT_TRUE(str1.empty());
}

TEST_F(BasicStringTest, EraseFromMiddle) {
    str3.erase(5, 6);
    EXPECT_STREQ(str3.c_str(), "Basic");
}

TEST_F(BasicStringTest, EraseAll) {
    str1.erase(0, str1.size());
    EXPECT_STREQ(str1.c_str(), "");
}

TEST_F(BasicStringTest, EraseInvalidIndex) {
    try {
        str1.erase(20, 5);
        FAIL() << "Expected std::out_of_range exception";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "Position is out of range.");
    }
}

TEST_F(BasicStringTest, EraseOutOfBounds) {
    try {
        str1.erase(20, 5);
        FAIL() << "Expected std::out_of_range exception";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "Position is out of range.");
    }
}

TEST_F(BasicStringTest, EraseFromStart) {
    str3.erase(0, 5);
    EXPECT_STREQ(str3.c_str(), "String");
}

TEST_F(BasicStringTest, Find) {
    size_t pos = str3.find("String");
    EXPECT_EQ(pos, 5);
}

TEST_F(BasicStringTest, CompareEqual) {
    BasicString<char> str_copy = str1;
    EXPECT_EQ(str1.compare(str_copy), 0);
}

TEST_F(BasicStringTest, CompareLessThan) {
    EXPECT_LT(str1.compare(str2), 0);
}

TEST_F(BasicStringTest, CompareGreaterThan) {
    EXPECT_LT(str4.compare(str3), 0);
}

TEST_F(BasicStringTest, StartsWith) {
    EXPECT_TRUE(str3.starts_with("Basic"));
    EXPECT_FALSE(str3.starts_with("String"));
}

TEST_F(BasicStringTest, EndsWith) {
    EXPECT_TRUE(str3.ends_with("String"));
    EXPECT_FALSE(str3.ends_with("Basic"));
}

TEST_F(BasicStringTest, StartsWithEmptyPrefix) {
    EXPECT_TRUE(str3.starts_with(""));
    EXPECT_TRUE(str3.starts_with("Bas"));
}

TEST_F(BasicStringTest, EndsWithEmptySuffix) {
    EXPECT_FALSE(str3.ends_with(""));
    EXPECT_TRUE(str3.ends_with("ing"));
}

TEST_F(BasicStringTest, GetAllocator) {
    auto allocator = str1.get_allocator();
}

TEST_F(BasicStringTest, Swap) {
    swap(str1, str2);
    EXPECT_STREQ(str1.c_str(), "World");
    EXPECT_STREQ(str2.c_str(), "Hello");
}

TEST_F(BasicStringTest, EqualityOperator) {
    BasicString<char> str_copy = str1;
    EXPECT_TRUE(str1 == str_copy);
}

TEST_F(BasicStringTest, InequalityOperator) {
    EXPECT_TRUE(str1 != str2);
}

TEST_F(BasicStringTest, LessThanOperator) {
    EXPECT_TRUE(str1 < str2);
}

TEST_F(BasicStringTest, GreaterThanOperator) {
    EXPECT_TRUE(str2 > str1);
}

TEST_F(BasicStringTest, LessThanOrEqualOperator) {
    EXPECT_TRUE(str1 <= str2);
}

TEST_F(BasicStringTest, GreaterThanOrEqualOperator) {
    EXPECT_TRUE(str2 >= str1);
}

TEST_F(BasicStringTest, StrongOrdering) {
    EXPECT_EQ(str1 <=> str2, std::strong_ordering::less);
    EXPECT_EQ(str2 <=> str1, std::strong_ordering::greater);
    EXPECT_EQ(str1 <=> str1, std::strong_ordering::equal);
}

TEST_F(BasicStringTest, AtThrowsOutOfRangeException) {
    try {
        str1.at(10);
        FAIL() << "Expected std::out_of_range exception";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "BasicString::at: position out of range");
    }
}

TEST_F(BasicStringTest, AtThrowsOutOfRangeForNegativeIndex) {
    try {
        str1.at(-1);
        FAIL() << "Expected std::out_of_range exception";
    } catch (const std::out_of_range& e) {
        EXPECT_STREQ(e.what(), "BasicString::at: position out of range");
    }
}

TEST_F(BasicStringTest, ClearForEmptyString) {
    BasicString<char> empty_str;
    empty_str.clear();
    EXPECT_EQ(empty_str.size(), 0);
    EXPECT_TRUE(empty_str.empty());
}

TEST_F(BasicStringTest, FindEmptySubstring) {
    size_t pos = str3.find("");
    EXPECT_EQ(pos, 0);
}

TEST_F(BasicStringTest, CompareEmptyStrings) {
    BasicString<char> empty1, empty2;
    EXPECT_EQ(empty1.compare(empty2), 0);
}

TEST_F(BasicStringTest, CompareEmptyStringWithNonEmpty) {
    BasicString<char> empty1;
    EXPECT_LT(empty1.compare(str1), 0);
    EXPECT_GT(str1.compare(empty1), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
