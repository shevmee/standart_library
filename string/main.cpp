#include "BasicString.hpp"
#include <iostream>

int main() {
    BasicString<wchar_t> str(L"Hello");
    std::wcout << str << std::endl;
}