#include "utils/utils.h"

#include <vector>
#include <unordered_map>

int main() {
    // 1. geo
    utils::PointT<int> ptA(10, 20), ptB(45, 55);
    utils::BoxT<int> box(ptA, ptB);
    std::cout << "Point A " << ptA << " and Point B " << ptB << " define Box " << box << ", which has an area of "
              << box.area() << "." << std::endl << std::endl;

    // 2. perttyprint
    std::vector<int> nums = {1, 2, 10};
    std::cout << "A list of integers: " << nums << std::endl;
    std::unordered_map<std::string, int> itemPrices;
    itemPrices["CPU"] = 2000;
    itemPrices["monitor"] = 2000;
    itemPrices["keyboard"] = 400;
    std::cout << "Prices of some items: " << itemPrices << std::endl << std::endl;

    // 3. log
    double a = 666.666;
    std::string b = "rounds to";
    int c = a;
    utils::print(a, b, c);
    utils::log() << "From now on, there is a time stamp in each line of log" << std::endl;
    utils::printlog("E.g.,", a, b, c);
    {
        int memStart = utils::mem_use::get_current();
        std::vector<int> manyNums(1000000);
        utils::printlog("A vector of 1000000 int takes", utils::mem_use::get_current() - memStart, "MB memory");
        utils::printlog("Memory: cur =", utils::mem_use::get_current(), "peak =", utils::mem_use::get_peak());
    }
    utils::printlog("Memory: cur =", utils::mem_use::get_current(), "peak =", utils::mem_use::get_peak());

    // 4. geo + prettyprint + log
    utils::printlog();
    std::vector<utils::PointT<double>> pts = {{1.2, 2.3}, {3.4, 4.5}, {5.4, 4.3}, {3.2, 2.1}};
    utils::printlog("There are", pts.size(), "points:", pts);
    utils::timer timeIt;
    utils::BoxT<double> bound;
    for (int i = 0; i < 100000; ++i) {
        bound.Set();
        for (const auto& pt : pts) {
            bound.Update(pt);
        }
    }
    utils::printlog("It takes", timeIt.elapsed() / 100000, "seconds to calcuate their bounding box", bound);
    utils::printlog("The above runtime is the average of", 100000, "runs");

    return 0;
}