#include <gtest/gtest.h>
#include "any.h"

TEST(AnyCastTest, IntCast) {
    any a(42);
    EXPECT_NO_THROW({
        int value = any_cast<int>(a);
        EXPECT_EQ(value, 42);
    });
}

TEST(AnyCastTest, StringCast) {
    any a(std::string("test"));
    EXPECT_NO_THROW({
        std::string value = any_cast<std::string>(a);
        EXPECT_EQ(value, "test");
    });
}

TEST(AnyCastTest, BadCast) {
    any a(42);
    EXPECT_THROW({
        any_cast<std::string>(a);
    }, std::bad_cast);
}

TEST(AnyCastTest, EmptyAnyCast) {
    any a;
    EXPECT_THROW({
        any_cast<int>(a);
    }, std::bad_cast);
}

TEST(AnyCopyTest, CopyConstruct) {
    any a(42);
    any b(a);
    EXPECT_NO_THROW({
        int value = any_cast<int>(b);
        EXPECT_EQ(value, 42);
    });
}

TEST(AnyMoveTest, MoveConstruct) {
    any a(42);
    any b(std::move(a));
    EXPECT_NO_THROW({
        int value = any_cast<int>(b);
        EXPECT_EQ(value, 42);
    });
}

TEST(AnyMoveTest, SourceAnyEmptyAfterMove) {
    any a(42);
    any b(std::move(a));
    EXPECT_TRUE(a.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
