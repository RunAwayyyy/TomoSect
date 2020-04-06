#pragma once

#include "enoki/array.h"

template <typename Value>
class approx
{
public:
    using Scalar = enoki::scalar_t<Value>;
    using Mask = enoki::mask_t<Value>;

    explicit approx(Value value) : epsilon_(std::numeric_limits<Scalar>::epsilon()), scale_(1), value_(value) {}

    approx operator()(Value value) const
    {
        approx<Value> approx(value);
        approx.epsilon(epsilon_);
        approx.scale(scale_);
        return approx;
    }

    approx& epsilon(Value newEpsilon)
    {
        epsilon_ = newEpsilon;
        return *this;
    }

    approx& scale(Value newScale)
    {
        scale_ = newScale;
        return *this;
    }

    template <typename V>
    friend Mask operator==(Value lhs, const approx<V>& rhs)
    {
        return enoki::abs(lhs - rhs.value_) < rhs.epsilon_ * (rhs.scale_ + enoki::max(enoki::abs(lhs), enoki::abs(rhs.value_)));
    }

    // clang-format off
    template <typename V> friend Mask operator==(const approx<V>& lhs, Value rhs) { return operator==(rhs, lhs); }
    template <typename V> friend Mask operator!=(Value lhs, const approx<V>& rhs) {  return !operator==(lhs, rhs); }
    template <typename V> friend Mask operator!=(const approx<V>& lhs, Value rhs) {  return !operator==(lhs, rhs); }
    template <typename V> friend Mask operator<=(Value lhs, const approx<V>& rhs) { return lhs < rhs.value_ || lhs == rhs; }
    template <typename V> friend Mask operator<=(const approx<V>& lhs, Value rhs) { return lhs.value_ < rhs || lhs == rhs; }
    template <typename V> friend Mask operator>=(Value lhs, const approx<V>& rhs) { return lhs > rhs.value_ || lhs == rhs; }
    template <typename V> friend Mask operator>=(const approx<V>& lhs, Value rhs) { return lhs.value_ > rhs || lhs == rhs; }
    template <typename V> friend Mask operator<(Value lhs, const approx<V>& rhs) { return lhs < rhs.value_ && lhs != rhs; }
    template <typename V> friend Mask operator<(const approx<V>& lhs, Value rhs) { return lhs.value_ < rhs && lhs != rhs; }
    template <typename V> friend Mask operator>(Value lhs, const approx<V>& rhs) { return lhs > rhs.value_ && lhs != rhs; }
    template <typename V> friend Mask operator>(const approx<V>& lhs, Value rhs) { return lhs.value_ > rhs && lhs != rhs; }

    // clang-format off
private:
    Value epsilon_;
    Value scale_;
    Value value_;
};