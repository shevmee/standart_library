#include "String.hpp"
#include <cassert>

int main() {
  String<std::allocator<char>> myString;

  String<> substring("Hello, world!", 0, 5);

  assert(strcmp(substring, "Hello") == 0);
}