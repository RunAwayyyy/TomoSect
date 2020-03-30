/**
 *
 * \file intersection.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include <tuple>
#include "TomoSect/geometry.hpp"

struct ray_rectangle_intersection {
};
struct ray_aabb_intersection {
};

// Intersection result, return a point and a mask, for valid hits
template<typename Value>
using IntersectionResult = std::tuple<point3<Value>, typename point3<Value>::Mask>;

template<typename Value>
IntersectionResult<Value>
intersection(const ray<Value> &ray, const aabb<Value> &aabb, ray_aabb_intersection) {
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
    return {tmp, mask};
}

template<typename Value>
IntersectionResult<Value>
intersection(const ray<Value> &ray, const rectangle<Value> &rect, ray_rectangle_intersection) {
    using mask_t = typename point3<Value>::Mask;

    Value denom = dot(rect.normal(), ray.dir());

    Value t = dot(rect.center() - ray.origin(), rect.normal()) / denom;

    mask_t mask = t > 0;

    vec3<Value> tmp = t * ray.dir();
    return {ray.origin() + tmp, mask};
}