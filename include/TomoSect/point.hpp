/**
 *
 * \file point.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include "enoki/array.h"
#include "enoki/random.h"

#include "TomoSect/vector.hpp"

template <size_t Size, typename Value> class point {
public:
  using Self = point<Size, Value>;
  using Type = enoki::value_t<Value>;
  using Vector = enoki::Array<Value, Size>;
  using Mask = enoki::mask_t<Value>;

  explicit point(Type all) : data_(all) {}

  template <typename... Args,
            typename std::enable_if_t<Size == sizeof...(Args), int> = 0>
  explicit point(Args... args) : data_(args...) {}

  // Construct a non homogeneous vector from a homogeneous one
  explicit point(enoki::Array<Value, Size + 1> homogeneous)
      : data_(enoki::head<Size>(homogeneous)) {}

  explicit point(vector<Size + 1, Value> homogeneous)
      : data_(enoki::head<Size>(homogeneous.data())) {}

  // Constructor with a Value, only if Value is an enoki::Array
  template <typename Array = Value,
            typename std::enable_if_t<enoki::is_array_v<Array>, int> = 0>
  explicit point(Array value) : data_(value) {}

  explicit point(Vector vec) : data_(vec) {}

  bool operator==(const Self &rhs) { return data_ == rhs.data_; }
  bool operator!=(const Self &rhs) { return data_ != rhs.data_; }

  Mask operator>(const Self &rhs) { return data_ > rhs.data_; }
  Mask operator>=(const Self &rhs) { return data_ >= rhs.data_; }
  Mask operator<(const Self &rhs) { return data_ < rhs.data_; }
  Mask operator<=(const Self &rhs) { return data_ <= rhs.data_; }

  template <size_t U = Size>
  typename std::enable_if_t<U >= 1, Value> x() const {
    return data_.x();
  }

  template <size_t U = Size>
  typename std::enable_if_t<U >= 2, Value> y() const {
    return data_.y();
  }

  template <size_t U = Size>
  typename std::enable_if_t<U >= 3, Value> z() const {
    return data_.z();
  }

  template <size_t U = Size>
  typename std::enable_if_t<U >= 4, Value> w() const {
    return data_.w();
  }

  Self &operator+=(const vector<Size, Value> &rhs) {
    data_ += rhs.data();
    return *this;
  }

  Self &operator-=(const vector<Size, Value> &rhs) {
    data_ -= rhs.data();
    return *this;
  }

  friend vector<Size, Value> operator-(const point<Size, Value> &lhs,
                                       const point<Size, Value> &rhs) {
    Vector v(lhs.data_);
    v -= rhs.data_;
    return vector<Size, Value>(v);
  }

  Vector data() const { return data_; }

  static Self Random() {
    using RNG_16x = enoki::PCG32<Vector>;
    RNG_16x my_rng;

    if constexpr (std::is_same_v<Type, float>) {
      Self point(my_rng.next_float32());

      return point;
    } else if constexpr (std::is_same_v<Type, int>) {
      Self point(my_rng.next_uint32());

      return point;
    }
  }

private:
  Vector data_;
};

// Coefficient wise operators

template <size_t Size, typename Value>
point<Size, Value> operator+(const point<Size, Value> &lhs,
                             const vector<Size, Value> &rhs) {
  auto copy = lhs;
  copy += rhs;
  return copy;
}

template <size_t Size, typename Value>
point<Size, Value> operator-(const point<Size, Value> &lhs,
                             const vector<Size, Value> &rhs) {
  auto copy = lhs;
  copy -= rhs;
  return copy;
}

template <typename Value> using point2 = point<2, Value>;

template <typename Value> using point3 = point<3, Value>;

template <typename Value> using point4 = point<4, Value>;