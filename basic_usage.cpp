#include <functional>
#include <iostream>

int square(int x)
{
    return x * x;
}

struct AddOne
{
    int operator() (int x)
    {
        return x + 1;
    }
};

int main()
{
    std::function<int(int)> f = &square;
    std::cout << f(5) << std::endl;


    f = AddOne();
    std::cout << f(5) << std::endl;


    f = [](int x)
    {
        return x * 10;
    };
    std::cout << f(5) << std::endl;

}
