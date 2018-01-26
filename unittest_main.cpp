#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils/utils.h"

using namespace utils;
using namespace std;

TEST_CASE("Interval", "[intvl]") {
    IntervalT<int> intvl1(0, 2);
    IntervalT<int> intvl2(2, 0);
    IntervalT<int> intvl3(10, 12);
    IntervalT<int> intvl4(12, 10);

    SECTION("invalid intervals") {
        REQUIRE(intvl1.IsValid());
        REQUIRE(!intvl2.IsValid());
        REQUIRE(intvl1 != intvl3);
        REQUIRE(intvl2 == intvl4);
    }

    SECTION("intersection/union on invalid intervals") {
        vector<IntervalT<int>> intvls = {intvl1, intvl2, intvl3, intvl4};
        for (int i = 0; i < intvls.size(); ++i) {
            for (int j = i; j < intvls.size(); ++j) {
                if (!intvls[i].IsValid() || !intvls[j].IsValid()) {
                    REQUIRE(!intvls[i].IntersectWith(intvls[j]).IsValid());
                    REQUIRE(!intvls[i].HasIntersectWith(intvls[j]));
                    REQUIRE(!intvls[i].UnionWith(intvls[j]).IsValid());
                }
            }
        }
    }

    SECTION("geo GetNearestPoints") {
        REQUIRE(intvl1.GetNearestPoint(-1) == 0);
        REQUIRE(intvl1.GetNearestPoint(1) == 1);
        REQUIRE(intvl1.GetNearestPoint(3) == 2);
        REQUIRE(intvl1.GetNearestPoints({1, 3}) == IntervalT<int>(1, 2));
        REQUIRE(intvl1.GetNearestPoints({3, 4}) == IntervalT<int>(2, 2));
    }
}