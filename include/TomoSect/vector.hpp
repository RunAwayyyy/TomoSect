/**
 *
 * \file vector.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include <enoki/random.h>
#include "enoki/array.h"

template <size_t Size, typename Value>
class vector {
public:
    using Self = vector<Size, Value>;
    using Type = enoki::value_t<Value>;
    using Vector = enoki::Array<Value, Size>;
    using Mask = enoki::mask_t<Value>;

    explicit vector(Type all) : data_(all) {}

    template<typename... Args,
            typename std::enable_if_t<Size == sizeof...(Args), int> = 0>
    explicit vector(Args ...args) : data_(args...) {}

    // Constructor with a Value, only if Value is an enoki::Array
    template<typename Array,
            typename std::enable_if_t<enoki::is_array_v<Array>, int> = 0>
    explicit vector(Array value) : data_(value) {}

    explicit vector(Vector vec) : data_(vec) {}

    bool operator==(const Self& rhs) { return data_ == rhs.data_; }
    bool operator!=(const Self& rhs) { return data_ != rhs.data_; }

    Mask operator>(const Self& rhs) { return data_ > rhs.data_; }
    Mask operator>=(const Self& rhs) { return data_ >= rhs.data_; }
    Mask operator<(const Self& rhs) { return data_ < rhs.data_; }
    Mask operator<=(const Self& rhs) { return data_ <= rhs.data_; }

    template <size_t U = Size>
    typename std::enable_if_t<U >= 1, Value>
    x() const { return data_.x(); }

    template <size_t U = Size>
    typename std::enable_if_t<U >= 2, Value>
    y() const { return data_.y(); }

    template <size_t U = Size>
    typename std::enable_if_t<U >= 3, Value>
    z() const { return data_.z(); }

    template <size_t U = Size>
    typename std::enable_if_t<U >= 4, Value>
    w() const { return data_.w(); }

    Self& operator+=(const Self& rhs) {
        data_ += rhs.data_;
        return *this;
    }

    Self& operator-=(const Self& rhs) {
        data_ -= rhs.data_;
        return *this;
    }

    Self& operator*=(const Self& rhs) {
        data_ *= rhs.data_;
        return *this;
    }

    Self& operator/=(const Self& rhs) {
        data_ /= rhs.data_;
        return *this;
    }

    template<typename V = Value>
    std::enable_if_t<enoki::is_array_v<V>, Self&>
    operator+=(const Value& rhs) {
        data_ += rhs;
        return *this;
    }

    template<typename V = Value>
    std::enable_if_t<enoki::is_array_v<V>, Self&>
    operator-=(const Value& rhs) {
        data_ -= rhs;
        return *this;
    }

    template<typename V = Value>
    std::enable_if_t<enoki::is_array_v<V>, Self&>
    operator*=(const Value& rhs) {
        data_ *= rhs;
        return *this;
    }

    template<typename V = Value>
    std::enable_if_t<enoki::is_array_v<V>, Self&>
    operator/=(const Value& rhs) {
        data_ /= rhs;
        return *this;
    }

    Self& operator+=(const Type& rhs) {
        data_ += rhs;
        return *this;
    }

    Self& operator-=(const Type& rhs) {
        data_ -= rhs;
        return *this;
    }

    Self& operator*=(const Type& rhs) {
        data_ *= rhs;
        return *this;
    }

    Self& operator/=(const Type& rhs) {
        data_ /= rhs;
        return *this;
    }

    Vector data() const { return data_; }

    static Self Random() {
        using RNG_16x = enoki::PCG32<Vector>;
        RNG_16x my_rng;

        Self point(my_rng.next_float32());

        return point;
    }

private:
    Vector data_;
};

// Util operators

template<size_t Size, typename Value>
vector<Size, Value> normalize(const vector<Size, Value>& lhs) {
    return vector<Size, Value>(enoki::normalize(lhs.data()));
}

template<size_t Size, typename Value>
vector<Size, Value> cross(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    return vector<Size, Value>(enoki::cross(lhs.data(), rhs.data()));
}

template<size_t Size, typename Value>
Value dot(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    return enoki::dot(lhs.data(), rhs.data());
}

// Coefficient wise operators

template<size_t Size, typename Value>
vector<Size, Value> operator+(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    auto copy = lhs;
    copy += rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator-(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    auto copy = lhs;
    copy -= rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator*(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    auto copy = lhs;
    copy *= rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator/(const vector<Size, Value>& lhs, const vector<Size, Value>& rhs) {
    auto copy = lhs;
    copy /= rhs;
    return copy;
}

// Vector-scalar operators

template<size_t Size, typename Value, typename std::enable_if_t<enoki::is_array_v<Value>, int> = 0>
vector<Size, Value> operator+(const vector<Size, Value>& lhs, const Value& rhs) {
    auto copy = lhs;
    copy += rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator+(const vector<Size, Value>& lhs, const typename vector<Size, Value>::Type& rhs) {
    auto copy = lhs;
    copy += rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator+(const typename vector<Size, Value>::Type& lhs, const vector<Size, Value>& rhs) {
    return rhs + lhs;
}

template<size_t Size, typename Value>
vector<Size, Value> operator-(const vector<Size, Value>& lhs, const typename vector<Size, Value>::Type& rhs) {
    auto copy = lhs;
    copy -= rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator-(const typename vector<Size, Value>::Type& lhs, const vector<Size, Value>& rhs) {
    return rhs - lhs;
}

template<size_t Size, typename Value>
vector<Size, Value> operator*(const vector<Size, Value>& lhs, const Value& rhs) {
    auto copy = lhs;
    copy += rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator*(const Value& lhs, const vector<Size, Value>& rhs) {
    return rhs + lhs;
}

template<size_t Size, typename Value>
vector<Size, Value> operator*(const vector<Size, Value>& lhs, const typename vector<Size, Value>::Type& rhs) {
    auto copy = lhs;
    copy *= rhs;
    return copy;
}

template<size_t Size, typename Value>
vector<Size, Value> operator*( const typename vector<Size, Value>::Type& lhs, const vector<Size, Value>& rhs) {
    return rhs * lhs;
}

template<size_t Size, typename Value>
vector<Size, Value> operator/(const vector<Size, Value>& lhs, const typename vector<Size, Value>::Type& rhs) {
    return vector<Size, Value>(lhs.data() / rhs);
}


template<size_t Size, typename Value>
vector<Size, Value> operator/(const typename vector<Size, Value>::Type& lhs, const vector<Size, Value>& rhs) {
    return vector<Size, Value>(1 / rhs.data());
}

template<typename Value>
using vec2 = vector<2, Value>;

template<typename Value>
using vec3 = vector<3, Value>;

template <typename Value>
using vec4 = vector<4, Value>;

template <typename T>
using pack2 = enoki::Packet<T, 2>;

template <typename T>
using pack4 = enoki::Packet<T, 4>;

template <typename T>
using pack8 = enoki::Packet<T, 8>;

template <typename T>
using pack16 = enoki::Packet<T, 16>;

template <typename T>
using pack32 = enoki::Packet<T, 32>;

template <typename T>
using pack64 = enoki::Packet<T, 64>;