#include "utils/utils.h"

#include <vector>

int main() {
    std::vector<int> prices = {10, 100, 20, 34};
    std::cout << prices << std::endl;
    print(prices);

    timer time;
    for (int i = 0; i < 10; ++i);
    std::cout << time;

    return 0;
}