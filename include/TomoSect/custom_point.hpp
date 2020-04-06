/**
 *
 * \file custom_point.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include "enoki/array.h"

#include <cstddef>
#include <type_traits>

namespace tomosect::details
{
    template <typename Test, template <typename...> class Ref>
    struct is_specialization : std::false_type {
    };

    template <template <typename...> class Ref, typename... Args>
    struct is_specialization<Ref<Args...>, Ref> : std::true_type {
    };

    template <typename T, template <typename...> class Ref>
    static constexpr bool is_specialization_v = is_specialization<T, Ref>::value;
} // namespace tomosect::details

namespace tomosect
{
    template <typename Value_, typename Derived_>
    struct PointBase {
    public:
        using Value = Value_;
        using Mask = enoki::mask_t<Value>;
        using Pack = typename Value_::Value;
        using Scalar = typename Value_::Scalar;

        /// Alias to the derived type
        using Derived = Derived_;

        /// Cast to derived type
        ENOKI_INLINE Derived& derived() { return (Derived&) *this; }

        /// Cast to derived type (const version)
        ENOKI_INLINE const Derived& derived() const { return (Derived&) *this; }

        // -----------------------------------------------------------------------
        //! @{ \name Implementations of vertical operations
        // -----------------------------------------------------------------------

        /// Addition
        auto add_(const Derived& a) const -> Value { return derived().data() + a.derived().data(); }

        /// Subtraction
        auto sub_(const Derived& a) const -> Value { return derived().data() - a.derived().data(); }

        /// Multiplication
        auto mul_(const Derived& a) const -> Value { return derived().data() * a.derived().data(); }

        /// Division
        auto div_(const Derived& a) const -> Value { return derived().data() / a.derived().data(); }

        /// Modulo
        auto mod_(const Derived& a) const -> Value { return derived().data() % a.derived().data(); }

        /// Equality comparison operation
        auto eq_(const Derived& a) const -> Mask { return enoki::eq(derived().data(), a.derived().data()); }

        /// Equality comparison operation
        auto neq_(const Derived& a) const -> Mask { return enoki::neq(derived().data(), a.derived().data()); }

        /// Less than comparison
        auto lt_(const Derived& a) const -> Mask { return derived().data() < a.derived().data(); }

        /// Less than or equal comparison
        auto le_(const Derived& a) const -> Mask { return derived().data() <= a.derived().data(); }

        /// Greater than comparison
        auto gt_(const Derived& a) const -> Mask { return derived().data() > a.derived().data(); }

        /// Greater than or equal comparison
        auto ge_(const Derived& a) const -> Mask { return derived().data() >= a.derived().data(); }

        /// Absolute value
        auto abs_() const -> Derived { return enoki::abs(derived().data()); }

        /// Square root
        auto sqrt_() const -> Derived { return enoki::sqrt(derived().data()); }

        /// Reciprocal
        auto rcp_() const -> Derived { return enoki::rcp(derived().data()); }

        /// Reciprocal square root
        auto rsqrt_() const -> Derived { return enoki::rsqrt(derived().data()); }

        /// Round to smallest integral value not less than argument
        auto ceil_() const -> Derived { return enoki::ceil(derived().data()); }

        /// Round to largest integral value not greater than argument
        auto floor_() const -> Derived { return enoki::floor(derived().data()); }

        /// Round to integral value
        auto round_() const -> Derived { return enoki::round(derived().data()); }

        /// Element-wise maximum
        auto max_() const -> Derived { return enoki::max(derived().data()); }

        /// Element-wise minimum
        auto min_() const -> Derived { return enoki::min(derived().data()); }

        /// Fused multiply-add
        auto fmadd_(const Derived& a1, const Derived& a2) const -> Derived
        {
            return enoki::fmadd(derived().data(), a1.derived().data(), a2.derived().data());
        }

        /// Fused negative multiply-add
        auto fnmadd_(const Derived& a1, const Derived& a2) const -> Derived
        {
            return enoki::fnmadd(derived().data(), a1.derived().data(), a2.derived().data());
        }

        /// Fused multiply-subtract
        auto fmsub_(const Derived& a1, const Derived& a2) const -> Derived
        {
            return enoki::fmsub(derived().data(), a1.derived().data(), a2.derived().data());
        }

        /// Fused negative multiply-subtract
        auto fnmsub_(const Derived& a1, const Derived& a2) const -> Derived
        {
            return enoki::fnmsub(derived().data(), a1.derived().data(), a2.derived().data());
        }

        //! @}
        // -----------------------------------------------------------------------
    };

    template <typename T>
    using is_point = enoki::is_base_of<PointBase, std::decay_t<T>>;

    template <typename T>
    static constexpr bool is_point_v = is_point<T>::value;

    template <typename T>
    using enable_if_point_t = enoki::enable_if_t<is_point_v<T>>;

    template <typename... Ts>
    constexpr bool is_point_any_v = std::disjunction<is_point<Ts>...>::value;

    template <typename... Ts>
    using enable_if_point_any_t = enoki::enable_if_t<is_point_any_v<Ts...>>;

    template <typename Value_, size_t Size_, typename Dervied_>
    struct PointImpl : PointBase<Value_, PointImpl<Value_, Size_, Dervied_>> {
        using Self = PointImpl<Value_, Size_, Dervied_>;

        using Base = PointBase<Value_, PointImpl<Value_, Size_, Dervied_>>;
        using typename Base::Derived;
        using typename Base::Value;
        using typename Base::Scalar;
        using typename Base::Mask;
        using typename Base::Pack;
        using Base::derived;

        PointImpl() = default;
        PointImpl(const PointImpl&) = default;
        PointImpl(PointImpl&&) noexcept = default;
        PointImpl& operator=(const PointImpl&) = default;
        PointImpl& operator=(PointImpl&&) = default;

        explicit PointImpl(Scalar s) : data_(s) {}
        explicit PointImpl(const Value& v) : data_(v) {}

        auto operator[](size_t index) const -> const Pack& { return coeff(index); }
        auto operator[](size_t index) -> Pack& { return coeff(index); }

        auto coeff(size_t index) const -> const Pack& { return data_.coeff(index); }
        auto coeff(size_t index) -> Pack& { return data_.coeff(index); }

        // -----------------------------------------------------------------------
        //! @{ \name Component access
        // -----------------------------------------------------------------------
        auto x() const -> const Pack&
        {
            static_assert(Size >= 1, "PointImpl::x(): require Size >= 1");
            return coeff(0);
        }
        auto x() -> Pack&
        {
            static_assert(Size >= 1, "PointImpl::x(): require Size >= 1");
            return coeff(0);
        }

        auto y() const -> const Pack&
        {
            static_assert(Size >= 2, "PointImpl::y(): require Size >= 2");
            return coeff(1);
        }
        auto y() -> Pack&
        {
            static_assert(Size >= 2, "PointImpl::y(): require Size >= 2");
            return coeff(1);
        }

        auto z() const -> const Pack&
        {
            static_assert(Size >= 3, "PointImpl::z(): require Size >= 3");
            return coeff(2);
        }
        auto z() -> Pack&
        {
            static_assert(Size >= 3, "PointImpl::z(): require Size >= 3");
            return coeff(2);
        }

        auto w() const -> const Pack&
        {
            static_assert(Size >= 4, "PointImpl::w(): require Size >= 4");
            return coeff(3);
        }
        auto w() -> Pack&
        {
            static_assert(Size >= 4, "PointImpl::w(): require Size >= 4");
            return coeff(3);
        }

        decltype(auto) data() { return data_; }
        decltype(auto) data() const { return data_; }

        //! @}
        // -----------------------------------------------------------------------

        /// Number of array entries
        static constexpr std::size_t Size = Size_;

        [[nodiscard]] constexpr size_t size() const { return Size; }

    private:
        Value data_;
    };

    template <typename T1, enable_if_point_any_t<T1> = 0>
    bool operator==(const T1 &a1, const T1& a2) {
        return enoki::operator==(a1.derived().data(), a2.derived().data());
    }

    //    template <typename T1, typename T2, enable_if_point_any_t<T1, T2> = 0>
    //    auto operator+(const T1& lhs, const T2& rhs)
    //    {
    //        if constexpr (std::is_integral_v<T1>)
    //            return rhs.add_(lhs);
    //        else if constexpr (std::is_integral_v<T2>)
    //            return lhs.add_(rhs);
    //        else
    //            return lhs.derived().add_(rhs.derived());
    //    }
    //
    //    template <typename T1, typename T2, enable_if_point_any_t<T1, T2> = 0>
    //    auto operator-(const T1& lhs, const T2& rhs)
    //    {
    //        if constexpr (std::is_integral_v<T1>)
    //            return rhs.sub_(lhs);
    //        else if constexpr (std::is_integral_v<T2>)
    //            return lhs.sub_(rhs);
    //        else
    //            return lhs.derived().sub_(rhs.derived());
    //    }
    //
    //    template <typename T1, typename T2, enable_if_point_any_t<T1, T2> = 0>
    //    auto operator*(const T1& lhs, const T2& rhs)
    //    {
    //        if constexpr (std::is_integral_v<T1>)
    //            return rhs.mul_(lhs);
    //        else if constexpr (std::is_integral_v<T2>)
    //            return lhs.mul_(rhs);
    //        else
    //            return lhs.derived().mul_(rhs.derived());
    //    }
    //
    //    template <typename T1, typename T2, enable_if_point_any_t<T1, T2> = 0>
    //    auto operator/(const T1& lhs, const T2& rhs)
    //    {
    //        if constexpr (std::is_integral_v<T1>)
    //            return lhs / rhs.derived().data();
    //        else if constexpr (std::is_integral_v<T2>)
    //            return lhs.div_(rhs);
    //        else
    //            return lhs.derived().div_(rhs.derived());
    //    }

    // Be able to do everything component wise and stuff
    template <typename Value_, size_t Size_>
    struct Array : PointImpl<Value_, Size_, Array<Value_, Size_>> {
        using Base = PointImpl<Value_, Size_, Array<Value_, Size_>>;

        Array() = default;
        Array(const Array&) = default;
        Array(Array&&) noexcept = default;
        Array& operator=(const Array&) = default;
        Array& operator=(Array&&) = default;

        using typename Base::Derived;
        using typename Base::Value;
        using typename Base::Scalar;

        using Base::Size;
        using Base::derived;

        using Base::Base;
        using Base::operator=;

        /* implicit */ Array(const Value& v) : Base(v) {}
    };

    // Be a classical mathematical vector
    template <typename Value_, size_t Size_>
    struct Vector : PointImpl<Value_, Size_, Vector<Value_, Size_>> {
        using Base = PointImpl<Value_, Size_, Vector<Value_, Size_>>;

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) noexcept = default;
        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        using typename Base::Derived;
        using typename Base::Value;
        using typename Base::Scalar;

        using Base::Size;
        using Base::derived;

        using Base::Base;
        using Base::operator=;

        /* implicit */ Vector(const Value& v) : Base(v) {}
    };

    // Be a mathematical point
    template <typename Value_, size_t Size_>
    struct Point : PointImpl<Value_, Size_, Vector<Value_, Size_>> {
        using Base = PointImpl<Value_, Size_, Vector<Value_, Size_>>;

        Point() = default;
        Point(const Point&) = default;
        Point(Point&&) noexcept = default;
        Point& operator=(const Point&) = default;
        Point& operator=(Point&&) = default;

        using typename Base::Derived;
        using typename Base::Value;
        using typename Base::Scalar;

        using Base::Size;
        using Base::derived;

        using Base::Base;
        using Base::operator=;

        /* implicit */ Point(const Value& v) : Base(v) {}
    };

    template <typename V1, size_t Size1, typename V2, size_t Size2>
    Vector<V1, Size1> operator-(const Point<V1, Size1>& lhs, const Point<V2, Size2>& rhs)
    {
        return lhs.derived().add_(rhs.derived());
    }

    template <typename V1, size_t Size1, typename V2, size_t Size2>
    Point<V1, Size1> operator+(const Point<V1, Size1>& lhs, const Vector<V2, Size2>& rhs)
    {
        return lhs.derived().add_(rhs.derived());
    }
    template <typename V1, size_t Size1, typename V2, size_t Size2>
    Point<V1, Size1> operator+(const Vector<V2, Size2>& lhs, const Point<V1, Size1> & rhs)
    {
        return rhs + lhs;
    }

    template <typename V1, size_t Size1, typename V2, size_t Size2>
    Point<V1, Size1> operator-(const Point<V1, Size1>& lhs, const Vector<V2, Size2>& rhs)
    {
        return lhs.derived().add_(rhs.derived());
    }
    template <typename V1, size_t Size1, typename V2, size_t Size2>
    Point<V1, Size1> operator-(const Vector<V2, Size2>& lhs, const Point<V1, Size1> & rhs)
    {
        return rhs + lhs;
    }

} // namespace tomosect
