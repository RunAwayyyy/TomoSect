/**
 *
 * \file test_point.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"

#include "TomoSect/point.hpp"
#include "enoki/cuda.h"

TEST_CASE("Testing point") {
    SUBCASE("Testing Construction") {
        SUBCASE("With a single value") {
            SUBCASE("2D Vector Cuda") {
                auto v = point2<enoki::CUDAArray<float>>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
            }
            SUBCASE("2D Vector") {
                auto v = point2<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
            }
            SUBCASE("3D Vector") {
                auto v = point3<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
                CHECK(v.z() == 1);
            }
            SUBCASE("3D Vector") {
                auto v = point4<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
                CHECK(v.z() == 1);
                CHECK(v.w() == 1);
            }
        }
    SUBCASE("With multiple values") {
        SUBCASE("2D Vector") {
            auto v = point2<float>(1, 2);

            CHECK(v.x() == 1);
            CHECK(v.y() == 2);
        }
        SUBCASE("3D Vector") {
            auto v = point3<float>(1, 2, 3);

            CHECK(v.x() == 1);
            CHECK(v.y() == 2);
            CHECK(v.z() == 3);
        }

        SUBCASE("4D Vector") {
            auto v = point4<float>(1, 2, 3, 4);

            CHECK(v.x() == 1);
            CHECK(v.y() == 2);
            CHECK(v.z() == 3);
            CHECK(v.w() == 4);
        }
    }
    SUBCASE("With a pack") {
        SUBCASE("2D Vector") {
            auto v1 = point2<pack2<float>>(pack2<float>{1, 2});

            CHECK(v1.x() == pack2<float>{1, 2});
            CHECK(v1.y() == pack2<float>{1, 2});
        }
    }
    SUBCASE("With multiple packs") {
        SUBCASE("2D Vector") {
            auto v1 = point2<pack2<float>>(pack2<float>{1, 2}, pack2<float>{3, 4});

            CHECK(v1.x() == pack2<float>{1, 2});
            CHECK(v1.y() == pack2<float>{3, 4});
        }
    }
    }
}