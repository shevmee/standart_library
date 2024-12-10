#include "String.hpp"
#include <iostream>

int main() {
    BasicString<char> str("HEllo");
    std::cout << str << std::endl;
}