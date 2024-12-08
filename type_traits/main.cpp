#include <vector>
#include "type_traits.h"

int main()
{
    std::vector<int> v;
    f(v);

    f(0);
}
