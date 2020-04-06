/**
 *
 * \file test_geometry.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"

#include "TomoSect/custom_point.hpp"
#include "enoki/array.h"
#include "Eigen/Eigen"

namespace ts = tomosect;

TEST_CASE("Conversion")
{
    using pack = enoki::Packet<float, 4>;
    using vec = enoki::Array<pack, 3>;

    ts::Point<vec, 3> p1{ 1 };
    ts::Point<vec, 3> p2{ 1 };

    ts::Vector<vec, 3> v1{ 1 };
    ts::Vector<vec, 3> v2{ 1 };

    ts::Vector<vec, 3> tmp1 = p1 - p1;
    ts::Point<vec, 3> tmp2 = v1 + p1;

    bool eq = p1 == p2;
    // auto tmp1 = 1 / p1;
//    auto tmp2 = p1 + v1;

//    auto tmp_v = v1 + 1;

    CHECK(p1.size() == 3);
    CHECK(p1.coeff(0) == 1);
}