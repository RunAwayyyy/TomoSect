/**
 *
 * \file test_intersection.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"

#include "TomoSect/intersection.hpp"

TEST_CASE("Intersection Ray Rectangle") {
  SUBCASE("Rectangle at center") {
    aabb box{point3<float>{0}, vec3<float>{1}};

    SUBCASE("Hit center") {
      auto r = ray(point3<float>{0.5, 0.5, -1}, vec3<float>{0, 0, 1});

      auto [hit_point, hit_mask] =
          intersection(r, box, ray_aabb_intersection{});

      CHECK(hit_mask);
      CHECK(hit_point.x() == 0.5);
      CHECK(hit_point.y() == 0.5);
      CHECK(hit_point.z() == 0);
    }
    SUBCASE("Hit off center") {
      auto r = ray(point3<float>{0.25, 0.25, -1}, vec3<float>{0, 0, 1});

      auto [hit_point, hit_mask] =
          intersection(r, box, ray_aabb_intersection{});

      CHECK(hit_mask);
      CHECK(hit_point.x() == 0.25);
      CHECK(hit_point.y() == 0.25);
      CHECK(hit_point.z() == 0);
    }
  }
}