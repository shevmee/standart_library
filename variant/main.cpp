#include <iostream>
#include <variant>

#include "variant.h"
/*
struct Leaf {};
struct Node;

using Tree = std::variant<Leaf, Node>;

struct Node
{
    Tree left;
    Tree right;
};

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};

int countLeaves(const Tree& tree)
{
    return std::visit(Overload{
        [](const Leaf&) { return 1; },
            [](this const auto& self, const Node* node) -> int
            {
                return std::visit(self, node->right) + std::visit(self, node->left);
            }
        }
    , tree);
}*/

int main()
{
    // Test integer variant
    variant<int, double, std::string> v1(42);
    std::cout << "v1 holds int: " << v1.get<int>() << std::endl;

    // Test double variant
    variant<int, double, std::string> v2(3.14);
    std::cout << "v2 holds double: " << v2.get<double>() << std::endl;

    // Test string variant
    variant<int, double, std::string> v3(std::string("hello"));
    std::cout << "v3 holds string: " << v3.get<std::string>() << std::endl;

    // Test copy constructor
    variant<int, double, std::string> v4(v3);
    std::cout << "v4 (copied from v3) holds string: " << v4.get<std::string>() << std::endl;

    // Test move constructor
    variant<int, double, std::string> v5(std::move(v4));
    std::cout << "v5 (moved from v4) holds string: " << v5.get<std::string>() << std::endl;

    // Test copy assignment
    v1 = v2;
    std::cout << "v1 (after assignment from v2) holds double: " << v1.get<double>() << std::endl;

    // Test move assignment
    v2 = std::move(v5);
    std::cout << "v2 (after move assignment from v5) holds string: " << v2.get<std::string>() << std::endl;

    // Test holds_alternative
    if (v2.holds_alternative<std::string>())
    {
        std::cout << "v2 holds a string: " << v2.get<std::string>() << std::endl;
    }
    else
    {
        std::cout << "v2 does not hold a string" << std::endl;
    }

    // Test exception throwing
    try
    {
        std::cout << "Trying to get int from v2: " << v2.get<int>() << std::endl;
    }
    catch (const std::bad_variant_access& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    };
}
