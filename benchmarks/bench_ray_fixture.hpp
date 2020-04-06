/**
 *
 * \file bench_ray_fixture.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include "celero/Celero.h"
#include <random>

std::random_device rdev;
std::uniform_real_distribution<float> dist(0, 1);

int num_rays = 1024 * 8;

template <typename Value>
class IntersectionFixture : public celero::TestFixture
{
public:
    class RayCountUDM : public celero::UserDefinedMeasurementTemplate<size_t>
    {
        virtual std::string getName() const override { return "Rays/sec"; }
        bool reportSize() const override { return false; }
        bool reportMean() const override { return true; }
        bool reportVariance() const override { return false; }
        bool reportStandardDeviation() const override { return true; }
        bool reportSkewness() const override { return false; }
        bool reportKurtosis() const override { return false; }
        bool reportZScore() const override { return false; }
        bool reportMin() const override { return true; }
        bool reportMax() const override { return true; }
    };

    IntersectionFixture() = default;

    double getExperimentValueResultScale() const override
    {
        if constexpr (!enoki::is_array_v<Value> || enoki::is_cuda_array_v<Value>) {
            return 1.0;
        } else {
            return Value::Size;
        }
    }

    [[nodiscard]] std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const override
    {
        std::vector<celero::TestFixture::ExperimentValue> problemSpace;

        problemSpace.emplace_back(int64_t(num_rays));

        return problemSpace;
    }

    /// Before each Sample, build a vector of random integers.
    void setUp(const celero::TestFixture::ExperimentValue& experimentValue) override
    {
        numRays_ = experimentValue.Value;
        rays_.reserve(numRays_);
        origin_ = point3<Value>{ 0, 0, -2.5 };

        for (int i = 0; i < numRays_; ++i) {
            rays_.emplace_back(origin_, vec3<Value>::Random());
        }
        aabb_ = aabb<Value>(point3<Value>(Value{ -0.5 }, Value{ -0.5 }, Value{ -1.5 }), vec3<Value>{ 1, 1, 1 });

        auto p = point2<Value>(10);
        auto t = point2<Value>(0);
        auto s = vec2<Value>(1);
        rect_ = rectangle<Value>(p, t, s);

        curved_rect_ = curved_rectangle<Value>(point2<Value>(1), point3<Value>(0, 0, -1));

        assert(rays_.size() == numRays_);
    }

    /// After each sample, clean up
    void tearDown() override
    {
        rays_.clear();

        // Calc (Number of rays * scale * iterations) / Time
        float scale = getExperimentValueResultScale();
        float iters = getExperimentIterations();
        float time = getExperimentTime() * 1e-6;
        rayCountUDM->addValue((numRays_ * scale * iters) / time);
    }

    virtual std::vector<std::shared_ptr<celero::UserDefinedMeasurement>> getUserDefinedMeasurements() const override
    {
        return { this->rayCountUDM };
    }

    int64_t numRays_{ 0 };
    std::vector<ray<Value>> rays_;
    point3<Value> origin_{ 0 };
    aabb<Value> aabb_;
    rectangle<Value> rect_;
    curved_rectangle<Value> curved_rect_;

    std::shared_ptr<RayCountUDM> rayCountUDM{ new RayCountUDM };
};