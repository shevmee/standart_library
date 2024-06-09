#include <gtest/gtest.h>
#include "function.h"

int square(int x) {
    return x * x;
}

auto lambda = [](int x) { return x + 1; };

struct Functor {
    int operator()(int x) const {
        return x - 1;
    }
};

// Тесты
TEST(FunctionTest, BasicFunction) {
    function<int(int)> f = square;
    EXPECT_EQ(f(5), 25);
}

TEST(FunctionTest, Lambda) {
    function<int(int)> f = lambda;
    EXPECT_EQ(f(5), 6);
}

TEST(FunctionTest, Functor) {
    Functor functor;
    function<int(int)> f = functor;
    EXPECT_EQ(f(5), 4);
}

TEST(FunctionTest, CopyConstructor) {
    function<int(int)> f1 = square;
    function<int(int)> f2 = f1;
    EXPECT_EQ(f2(5), 25);
}

TEST(FunctionTest, MoveConstructor) {
    function<int(int)> f1 = square;
    function<int(int)> f2 = std::move(f1);
    EXPECT_EQ(f2(5), 25);
    EXPECT_THROW(f1(5), std::bad_function_call);
}

TEST(FunctionTest, CopyAssignment) {
    function<int(int)> f1 = square;
    function<int(int)> f2;
    f2 = f1;
    EXPECT_EQ(f2(5), 25);
}

TEST(FunctionTest, MoveAssignment) {
    function<int(int)> f1 = square;
    function<int(int)> f2;
    f2 = std::move(f1);
    EXPECT_EQ(f2(5), 25);
    EXPECT_THROW(f1(5), std::bad_function_call);
}

TEST(FunctionTest, EmptyFunction) {
    function<int(int)> f;
    EXPECT_FALSE(f);
    EXPECT_THROW(f(5), std::bad_function_call);
}

TEST(FunctionTest, BoolConversion) {
    function<int(int)> f;
    EXPECT_FALSE(f);
    f = square;
    EXPECT_TRUE(f);
}

TEST(FunctionBindTest, BindCompatibility) {
    auto bound_func = std::bind([](int a, int b) { return a + b; }, 1, 2);
    function<int()> f(bound_func);
    EXPECT_EQ(f(), 3);  // Ожидаем, что результат будет 3
}

// Тест для проверки bind с передачей параметров
TEST(FunctionBindTest, BindWithParameters) {
    auto bound_func = std::bind([](int a, int b) { return a + b; }, std::placeholders::_1, 2);
    function<int(int)> f(bound_func);
    EXPECT_EQ(f(3), 5); // Ожидаем, что результат будет 5, т.к. 3 + 2 = 5
}

// Основная функция для запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
