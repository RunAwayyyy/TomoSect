/**
 *
 * \file intersection.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include "TomoSect/geometry.hpp"
#include "TomoSect/approx.hpp"

#include <tuple>

struct ray_rectangle_intersection {
};
struct ray_curved_rectangle_intersection {
};
struct ray_aabb_intersection {
};

// Intersection result, return a point and a mask, for valid hits
template <typename Value>
using IntersectionResult = std::tuple<point3<Value>, typename point3<Value>::Mask>;

template <typename Value>
IntersectionResult<Value> intersection(const ray<Value>& ray, const aabb<Value>& aabb, ray_aabb_intersection)
{
    using mask_t = typename point3<Value>::Mask;

    using Vector = typename vec3<Value>::Vector;

    vec3<Value> inv_dir = 1 / ray.dir();

    vec3<Value> t0s = (aabb.min() - ray.origin()) * inv_dir;
    vec3<Value> t1s = (aabb.max() - ray.origin()) * inv_dir;

    Vector t_mins = enoki::min(t0s.data(), t1s.data());
    Vector t_maxs = enoki::max(t0s.data(), t1s.data());

    Value tmin = enoki::hmax(t_mins);
    Value tmax = enoki::hmin(t_maxs);

    mask_t mask = tmin < tmax;

    auto tmp = point3<Value>(ray.origin().data() + ray.dir().data() * tmin);
    return { tmp, mask };
}

template <typename Value, bool testAABB = false>
IntersectionResult<Value> intersection(const ray<Value>& ray, const curved_rectangle<Value>& rect, ray_curved_rectangle_intersection)
{
    using mask_t = typename point3<Value>::Mask;
    using Vector = typename vec3<Value>::Vector;

    if constexpr (testAABB) {
        auto box = rect.getAABB();

        auto [hit_point, hit_mask] = intersection(ray, box, ray_aabb_intersection{});

        // We can only exit, if none of the ray hits!
        if (enoki::none(hit_mask))
            return { hit_point, hit_mask };
    }

    // Convert ray origin and direction to local coords
    auto ro = rect.toLocal(ray.origin()).data();
    auto rd = rect.toLocal(ray.dir()).data();
    rd = enoki::normalize(rd);

    auto ro2d = enoki::Array<Value, 2>{ ro.x(), ro.z() };
    auto rd2d = enoki::Array<Value, 2>{ rd.x(), rd.z() };

    // setup quadratic equation
    auto a = enoki::dot(rd2d, rd2d);
    auto b = 2 * rd.x() * ro.x() + 2 * rd.z() * ro.z();
    auto c = enoki::dot(ro2d, ro2d) - Value{ 1 };

    auto determinant = enoki::pow(b, 2) - (4 * a * c);

    // Solve quadratic equation
    auto t1 = enoki::select(determinant > 0, (-b + enoki::sqrt(determinant)) / (2 * a), -1);
    auto t2 = enoki::select(determinant > 0, (-b - enoki::sqrt(determinant)) / (2 * a), -1);

    // find minimum
    auto tmin = enoki::min(t1, t2);
    auto tmax = enoki::max(t1, t2);

    // take max, if min value is smaller than 0
    auto t = enoki::select(tmin > 0, tmin, tmax);

    // Calculate local point (it's always 1 unit away, so we don't need the t for the calculation)
    auto point = point3<Value>(ro + rd);
    auto global = rect.toGlobal(point);

    // Convert local point to cylindrical coordinates
    auto cylindrical = toCylindrical(point);
    auto angle = rect.angle();

    auto non_negative = t > 0;    // t should be positive
    auto zmin = point.z() > -0.5; // local point's z value should be above in [-0.5; 0.5]
    auto zmax = point.z() < 0.5;
    auto in_segment =
        approx(cylindrical.y()).epsilon(0.001) <= angle; // the theta of the cylindrical coord, should be smaller than the theta
    mask_t mask = non_negative && zmin && zmax && in_segment;

    /*
    std::cout << "Mask: " << mask << "\n";
    std::cout << "Point: " << point.data() << "\n";
    std::cout << "Point global: " << global.data() << "\n";
    std::cout << "ray.origin().data(): " << ray.origin().data() << "\n";
    std::cout << "ray.dir().data(): " << ray.dir().data() << "\n";
    */

    return { global, mask };
}

template <typename Value>
IntersectionResult<Value> intersection(const ray<Value>& ray, const rectangle<Value>& rect, ray_rectangle_intersection)
{
    using mask_t = typename point3<Value>::Mask;

    Value denom = dot(rect.normal(), ray.dir());

    Value t = dot(rect.center() - ray.origin(), rect.normal()) / denom;

    mask_t mask = t > 0;

    vec3<Value> tmp = t * ray.dir();
    return { ray.origin() + tmp, mask };
}