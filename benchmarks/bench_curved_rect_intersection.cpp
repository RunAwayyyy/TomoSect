/**
 *
 * \file bench_aabb_intersection.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include <celero/Celero.h>

#include <random>

#include "enoki/cuda.h"

#include "TomoSect/geometry.hpp"
#include "TomoSect/intersection.hpp"

#include "bench_ray_fixture.hpp"

int main(int argc, char** argv)
{
    celero::Run(argc, argv);
    return 0;
}

int SAMPLES = 10;
int ITERATIONS = 1000;

BASELINE_F(HitAABB, NoPack, IntersectionFixture<float>, SAMPLES, ITERATIONS)
{
    for (auto r : rays_) {
        auto hit = intersection(r, curved_rect_, ray_curved_rectangle_intersection{});
        celero::DoNotOptimizeAway(hit);
    }
}

BENCHMARK_F(HitAABB, PackOf2, IntersectionFixture<pack2<float>>, SAMPLES, ITERATIONS)
{
    for (auto r : rays_) {
        auto hit = intersection(r, curved_rect_, ray_curved_rectangle_intersection{});
        celero::DoNotOptimizeAway(hit);
    }
}

BENCHMARK_F(HitAABB, PackOf4, IntersectionFixture<pack4<float>>, SAMPLES, ITERATIONS)
{
    for (auto r : rays_) {
        auto hit = intersection(r, curved_rect_, ray_curved_rectangle_intersection{});
        celero::DoNotOptimizeAway(hit);
    }
}

BENCHMARK_F(HitAABB, PackOf8, IntersectionFixture<pack8<float>>, SAMPLES, ITERATIONS)
{
    for (auto r : rays_) {
        auto hit = intersection(r, curved_rect_, ray_curved_rectangle_intersection{});
        celero::DoNotOptimizeAway(hit);
    }
}