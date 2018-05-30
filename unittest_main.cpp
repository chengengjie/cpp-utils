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
        REQUIRE(L2Dist(pt1, pt2) == sqrt(500));
        REQUIRE(LInfDist(pt1, pt2) == 20);
    }
}

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
                    // at least one invalid
                    REQUIRE(!intvls[i].IntersectWith(intvls[j]).IsValid());
                    REQUIRE(!intvls[i].HasIntersectWith(intvls[j]));
                }
                REQUIRE(intvls[i].UnionWith(intvls[j]).IsValid() == (intvls[i].IsValid() || intvls[j].IsValid()));
            }
        }
    }

    SECTION("interval GetNearestPoint(s)To") {
        REQUIRE(intvl1.GetNearestPointTo(-1) == 0);
        REQUIRE(intvl1.GetNearestPointTo(1) == 1);
        REQUIRE(intvl1.GetNearestPointTo(3) == 2);
        REQUIRE(intvl1.GetNearestPointsTo({1, 3}) == IntervalT<int>(1, 2));
        REQUIRE(intvl1.GetNearestPointsTo({3, 4}) == IntervalT<int>(2, 2));
    }

    SECTION("interval dist") {
        REQUIRE(Dist(intvl1, -1) == 1);
        REQUIRE(Dist(intvl1, 1) == 0);
        REQUIRE(Dist(intvl1, 3) == 1);
        REQUIRE(Dist(intvl1, {1, 3}) == 0);
        REQUIRE(Dist(intvl1, {3, 4}) == 1);
    }
}

TEST_CASE("Box", "[box]") {
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
        REQUIRE(box9.lx() == 4);
        REQUIRE(box9.area() == 4);
    }

    BoxT<int> boxA(4, 5, 6, 7);
    BoxT<int> boxB(14, 15, 16, 17);
    BoxT<int> boxC(4, 15, 6, 17);

    SECTION("box GetNearestPoint(s)To") {
        REQUIRE(boxA.GetNearestPointTo({5, 6}) == PointT<int>(5, 6));
        REQUIRE(boxA.GetNearestPointTo({5, 13}) == PointT<int>(5, 7));
        REQUIRE(boxA.GetNearestPointsTo(boxB) == BoxT<int>(6, 7));
        REQUIRE(boxA.GetNearestPointsTo(boxC) == BoxT<int>(4, 7, 6, 7));
    }

    SECTION("box dist") {
        REQUIRE(Dist(boxA, PointT<int>(5, 6)) == 0);
        REQUIRE(Dist(boxA, PointT<int>(5, 13)) == 6);
        REQUIRE(Dist(boxA, boxB) == 16);
        REQUIRE(Dist(boxA, boxC) == 8);
        REQUIRE(L2Dist(boxA, boxB) == sqrt(128));
        REQUIRE(L2Dist(boxA, boxC) == 8);
    }

    BoxT<int> boxD(5, 6, 15, 16);
    BoxT<int> boxE(9, 100, 10, 102);

    SECTION("slice polygons") {
        vector<BoxT<int>> boxes = {boxA, boxB, boxD, boxE};
        SlicePolygons(boxes, 1);
        REQUIRE(boxes.size() == 6);
    }
}