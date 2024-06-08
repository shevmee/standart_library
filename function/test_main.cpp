#include <gtest/gtest.h>
#include "function.h"

// Простой тест для проверки вызова функции
TEST(FunctionTest, SimpleCall) {
    function<int(int)> f = [](int x) { return x + 1; };
    EXPECT_EQ(f(1), 2);
}

// Тест для проверки копирования
TEST(FunctionTest, Copy) {
    function<int(int)> f1 = [](int x) { return x + 1; };
    function<int(int)> f2 = f1;
    EXPECT_EQ(f2(1), 2);
}

// Тест для проверки перемещения
TEST(FunctionTest, Move) {
    function<int(int)> f1 = [](int x) { return x + 1; };
    function<int(int)> f2 = std::move(f1);
    EXPECT_EQ(f2(1), 2);
    EXPECT_FALSE(static_cast<bool>(f1));  // f1 должен быть пустым после перемещения
}

// Тест для проверки копирующего присваивания
TEST(FunctionTest, CopyAssignment) {
    function<int(int)> f1 = [](int x) { return x + 1; };
    function<int(int)> f2;
    f2 = f1;
    EXPECT_EQ(f2(1), 2);
}

// Тест для проверки перемещающего присваивания
TEST(FunctionTest, MoveAssignment) {
    function<int(int)> f1 = [](int x) { return x + 1; };
    function<int(int)> f2;
    f2 = std::move(f1);
    EXPECT_EQ(f2(1), 2);
    EXPECT_FALSE(static_cast<bool>(f1));  // f1 должен быть пустым после перемещения
}

// Тест для проверки явного преобразования к bool
TEST(FunctionTest, BoolConversion) {
    function<int(int)> f;
    EXPECT_FALSE(static_cast<bool>(f));  // f пустой, должно быть false

    f = [](int x) { return x + 1; };
    EXPECT_TRUE(static_cast<bool>(f));  // f содержит функтор, должно быть true
}

// Основная функция для запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
