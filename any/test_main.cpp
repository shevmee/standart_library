#include <gtest/gtest.h>
#include "any.h"

// Успешное приведение типа int
TEST(AnyCastTest, IntCast) {
    any a(42);
    EXPECT_NO_THROW({
        int value = any_cast<int>(a);
        EXPECT_EQ(value, 42);
    });
}

// Успешное приведение типа std::string
TEST(AnyCastTest, StringCast) {
    any a(std::string("test"));
    EXPECT_NO_THROW({
        std::string value = any_cast<std::string>(a);
        EXPECT_EQ(value, "test");
    });
}

// Приведение к неверному типу должно выбрасывать исключение std::bad_cast
TEST(AnyCastTest, BadCast) {
    any a(42);
    EXPECT_THROW({
        any_cast<std::string>(a);
    }, std::bad_cast);
}

// Приведение пустого any должно выбрасывать исключение std::bad_cast
TEST(AnyCastTest, EmptyAnyCast) {
    any a;
    EXPECT_THROW({
        any_cast<int>(a);
    }, std::bad_cast);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
