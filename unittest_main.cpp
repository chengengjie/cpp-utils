#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils/utils.h"

using namespace utils;
using namespace std;

TEST_CASE("Point", "[pt]") {
    PointT<int> pt1(0, 1);
    PointT<int> pt2(10, 21);

    SECTION("point distance") {
        REQUIRE(Dist(pt1, pt2) == 30);
        REQUIRE(LInfDist(pt1, pt2) == 20);
    }
}

TEST_CASE("Interval/Box", "[intvl/box]") {
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

    SECTION("interval/box GetNearestPoints") {
        REQUIRE(intvl1.GetNearestPointTo(-1) == 0);
        REQUIRE(intvl1.GetNearestPointTo(1) == 1);
        REQUIRE(intvl1.GetNearestPointTo(3) == 2);
        REQUIRE(intvl1.GetNearestPointsTo({1, 3}) == IntervalT<int>(1, 2));
        REQUIRE(intvl1.GetNearestPointsTo({3, 4}) == IntervalT<int>(2, 2));
    }

    SECTION("interval distance") {
        REQUIRE(Dist(intvl1, intvl3) == 8);
        REQUIRE(Dist(intvl3, intvl1) == 8);
        REQUIRE(Dist(intvl1, intvl2) == numeric_limits<int>::max());
        REQUIRE(Dist(intvl2, intvl1) == numeric_limits<int>::max());
    }

    SECTION("box ctor") {
        BoxT<int> box;
        REQUIRE(!box.IsValid());

        BoxT<int> box1(4, 5);
        BoxT<int> box2(PointT<int>(4, 5));
        BoxT<int> box3(4, 5, 4, 5);
        BoxT<int> box4(IntervalT<int>(4), IntervalT<int>(5));
        BoxT<int> box5(PointT<int>(4, 5), PointT<int>(4, 5));
        BoxT<int> box6(box5);
        REQUIRE(box1 == box2);
        REQUIRE(box1 == box3);
        REQUIRE(box1 == box4);
        REQUIRE(box1 == box5);
        REQUIRE(box1 == box6);

        BoxT<int> box7(4, 5, 6, 7);
        BoxT<int> box8(IntervalT<int>(4, 6), IntervalT<int>(5, 7));
        BoxT<int> box9(PointT<int>(4, 5), PointT<int>(6, 7));
        REQUIRE(box7 == box8);
        REQUIRE(box7 == box9);
    }
}