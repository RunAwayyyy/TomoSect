/**
 *
 * \file test_intersection.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"

#include "TomoSect/intersection.hpp"

TEST_CASE("Intersection Ray AABB")
{
    SUBCASE("Rectangle at center")
    {
        /*aabb box{ point3<float>{ 0 }, vec3<float>{ 1 } };

        SUBCASE("Hit center")
        {
            auto r = ray(point3<float>{ 0.5, 0.5, -1 }, vec3<float>{ 0, 0, 1 });

            auto [hit_point, hit_mask] = intersection(r, box, ray_aabb_intersection{});

            CHECK(hit_mask);
            CHECK(hit_point.x() == 0.5);
            CHECK(hit_point.y() == 0.5);
            CHECK(hit_point.z() == 0);
        }
        SUBCASE("Hit off center")
        {
            auto r = ray(point3<float>{ 0.25, 0.25, -1 }, vec3<float>{ 0, 0, 1 });

            auto [hit_point, hit_mask] = intersection(r, box, ray_aabb_intersection{});

            CHECK(hit_mask);
            CHECK(hit_point.x() == 0.25);
            CHECK(hit_point.y() == 0.25);
            CHECK(hit_point.z() == 0);
        }*/
    }
}

TEST_CASE("Intersection Ray Curved Rectangle")
{
    SUBCASE("Rectangle at center")
    {
        using pack = float;
        auto rect = curved_rectangle(point2<pack>(1), point3<pack>(0, 0, -1), 1, degree<pack>(90));

        SUBCASE("Hit center / principal point")
        {
            auto r = ray(point3<pack>{ 0, 0, 0 }, vec3<pack>{ 0, 0, -1 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

            CHECK(enoki::all(hit_mask));
            CHECK(hit_point.x() == 0);
            CHECK(hit_point.y() == 0);
            CHECK(hit_point.z() == -1);
        }
        SUBCASE("Miss, by going above detector")
        {
            auto r = ray(point3<pack>{ 0, 0, 0 }, vec3<pack>{ 0, 1, -1 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

            CHECK(enoki::none(hit_mask));
        }
        SUBCASE("Miss, by going to the left")
        {
            auto r = ray(point3<pack>{ 0, 0, 0 }, vec3<pack>{ 1, 0, 0 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

            CHECK(enoki::none(hit_mask));
        }
        SUBCASE("Hit at left edge")
        {
            auto r = ray(point3<pack>{ 0, 0, 0 }, vec3<pack>{ 1, 0, -1 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

            CHECK(enoki::all(hit_mask));
            CHECK(enoki::all(hit_point.x() == approx(enoki::cos(degree<pack>(45).to_radian()))));
            CHECK(hit_point.y() == 0);
            CHECK(enoki::all(hit_point.z() == approx(-enoki::cos(degree<pack>(45).to_radian()))));
        }
        SUBCASE("Hit at right edge")
        {
            auto r = ray(point3<pack>{ 0, 0, 0 }, vec3<pack>{ -1, 0, -1 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

                CHECK(enoki::all(hit_mask));
                CHECK(enoki::all(hit_point.x() == approx(-enoki::cos(degree<pack>(45).to_radian()))));
                CHECK(hit_point.y() == 0);
                CHECK(enoki::all(hit_point.z() == approx(-enoki::cos(degree<pack>(45).to_radian()))));
        }
        SUBCASE("Hit center / principal point")
        {
            auto r = ray(point3<pack>{ 0, 0, -2 }, vec3<pack>{ 0, 0, 1 });

            auto [hit_point, hit_mask] = intersection(r, rect, ray_curved_rectangle_intersection{});

            CHECK(enoki::all(hit_mask));
            CHECK(hit_point.x() == 0);
            CHECK(hit_point.y() == 0);
            CHECK(hit_point.z() == -1);
        }
    }
}