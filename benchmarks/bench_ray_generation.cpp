/**
 *
 * \file bench_ray_generation.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include <celero/Celero.h>

#include <random>

#include "enoki/cuda.h"

#include "TomoSect/geometry.hpp"
#include "TomoSect/intersection.hpp"

#include "bench_ray_fixture.hpp"

int main(int argc, char **argv) {
  celero::Run(argc, argv);
  return 0;
}

int SAMPLES = 10;
int ITERATIONS = 10;

BASELINE_F(RayGeneration, NoPack, IntersectionFixture<float>, SAMPLES,
           ITERATIONS) {
  using Value = float;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, PackOf2, IntersectionFixture<pack2<float>>, SAMPLES,
            ITERATIONS) {
  using Value = pack2<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, PackOf4, IntersectionFixture<pack4<float>>, SAMPLES,
            ITERATIONS) {
  using Value = pack4<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, PackOf8, IntersectionFixture<pack8<float>>, SAMPLES,
            ITERATIONS) {
  using Value = pack8<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, PackOf16, IntersectionFixture<pack16<float>>,
            SAMPLES, ITERATIONS) {
  using Value = pack16<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, PackOf32, IntersectionFixture<pack32<float>>,
            SAMPLES, ITERATIONS) {
  using Value = pack32<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}

BENCHMARK_F(RayGeneration, CudaArray,
            IntersectionFixture<enoki::CUDAArray<float>>, SAMPLES, ITERATIONS) {
  using Value = enoki::CUDAArray<float>;
  for (int x = 0; x < 10; ++x) {
    for (int y = 0; y < 10; y++) {
      auto planeCoord = rect_.coordFromLocal(point2<Value>{x, y});

      auto r = rayFromPoints(origin_, planeCoord);

      auto hit = intersection(r, aabb_, ray_aabb_intersection{});

      celero::DoNotOptimizeAway(hit);
    }
  }
}