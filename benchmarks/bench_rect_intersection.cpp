/**
 *
 * \file bench_test.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include <celero/Celero.h>

#include <random>

#include "TomoSect/geometry.hpp"
#include "TomoSect/intersection.hpp"

#include "bench_ray_fixture.hpp"

int main(int argc, char **argv) {
  celero::Run(argc, argv);
  return 0;
}

int SAMPLES = 20;
int ITERATIONS = 10000;

BASELINE_F(HitRectangle, NoPack, IntersectionFixture<float>, SAMPLES,
           ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf2, IntersectionFixture<pack2<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf4, IntersectionFixture<pack4<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf8, IntersectionFixture<pack8<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf16, IntersectionFixture<pack16<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf32, IntersectionFixture<pack32<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}

BENCHMARK_F(HitRectangle, PackOf64, IntersectionFixture<pack64<float>>, SAMPLES,
            ITERATIONS) {
  for (auto r : rays_) {
    auto hit = intersection(r, rect_, ray_rectangle_intersection{});
    celero::DoNotOptimizeAway(hit);
  }
}