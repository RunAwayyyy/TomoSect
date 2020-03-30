/**
 *
 * \file geometry.hpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#pragma once

#include "enoki/matrix.h"
#include "enoki/transform.h"

#include "TomoSect/vector.hpp"
#include "TomoSect/point.hpp"

#include <cmath>

template<typename Value>
class degree;

template<typename Value>
class radian {
public:
    using Scalar = enoki::scalar_t<Value>;

    explicit radian(Value radian) : radian_(radian) {}

    radian(degree<Value> d) : radian_(d.to_radian()) {}

    Value to_degree() const { return radian_ * 180 / M_PI; }

    Value get() const { return radian_; }

private:
    Value radian_;
};

template<typename Value>
class degree {
public:
    using Scalar = enoki::scalar_t<Value>;

    explicit degree(Value a) : degree_(a) {}

    degree(radian<Value> r) : degree_(r.to_degree()) {}

    Value to_radian() const { return degree_ * M_PI / 180; }

    Value get() const { return degree_; }

private:
    Value degree_;
};

template<typename Value>
class ray {
public:
    struct ray_direction_normalized {
    };

    ray(const point3<Value> &o, const vec3<Value> &d) : origin_(o), direction_(normalize(d)) {}

    ray(const point3<Value> &o, const vec3<Value> &d, ray_direction_normalized) : origin_(o), direction_(d) {}

    point3<Value> origin() const { return origin_; }

    vec3<Value> dir() const { return direction_; }

private:
    point3<Value> origin_;
    vec3<Value> direction_;
};

template<typename Value>
ray<Value> rayFromPoints(const point3<Value> &p1, const point3<Value> &p2) {
    // Create ray from p1 to p2, with p1 as origin
    return ray<Value>{p1, p2 - p1};
}

template<typename Matrix, typename Vector3, enoki::enable_if_t<Matrix::IsMatrix && Matrix::Size == 4> = 0>
std::tuple<Matrix, Matrix>
affine_transformation(const Vector3 &translation, const Vector3 &scale, radian<enoki::entry_t<Matrix>> rotX,
                      radian<enoki::entry_t<Matrix>> rotY, radian<enoki::entry_t<Matrix>> rotZ) {
    // Rotate around x-axis, y-axis and z-axis
    Matrix matrixRotX = enoki::rotate<Matrix>(Vector3(1, 0, 0), rotX.get());
    Matrix matrixRotY = enoki::rotate<Matrix>(Vector3(0, 1, 0), rotY.get());
    Matrix matrixRotZ = enoki::rotate<Matrix>(Vector3(0, 0, 1), rotZ.get());

    // Scale, we want larger numbers to scale up, therefore inverte here
    Matrix matrixScale = enoki::scale<Matrix>(1 / scale);

    // Translate (assume bottom left is at 0.5, 0.5, 0)
    Matrix matrixTranslate = enoki::translate<Matrix>(Vector3{0.5, 0.5, 0} - translation);

    // From global to local, so scale, then rotate, then move
    auto inv_ = matrixTranslate * matrixRotZ * matrixRotY * matrixRotX * matrixScale;

    // From local to global
    auto m_ = enoki::inverse(inv_);

    // std::cout << "m_:\n" << m_ << "\n";
    // std::cout << "inv_:\n" << inv_ << "\n";

    return {m_, inv_};
}

template<typename Matrix, typename Vector3, enoki::enable_if_t<Matrix::IsMatrix && Matrix::Size == 4> = 0>
std::tuple<Matrix, Matrix>
cylindrical_transformation(const Vector3 &center, const Vector3 &principalPoint, enoki::entry_t<Matrix> height_scale) {
    using Vector4 = enoki::column_t<Matrix>;

    // Radius is used to scale, from here we assume radius = 1
    auto radius = enoki::norm(center - principalPoint);

    auto matrixScale = enoki::scale<Matrix>(Vector3{radius, radius, height_scale});

    // We assume detector is in z direction, x is towards detector and y "right"
    auto rotate = Matrix::from_rows(
            Vector4{  0, 1, 0, 0 },
            Vector4{  0, 0, 1, 0 },
            Vector4{ -1, 0, 0, 0 },
            Vector4{  0, 0, 0, 1 }
    );

    auto look_at = enoki::look_at<Matrix>(Vector3{0}, principalPoint - center, Vector3{0, 1, 0});

    auto matrixTranslate = enoki::translate<Matrix>(center);

    // From global to local, so scale, then rotate, then move
    auto m = matrixTranslate * look_at * rotate * matrixScale;

    // From local to global
    auto inv = enoki::inverse(m);

    /*std::cout << "matrixTranslate:\n" << matrixTranslate << "\n";
    std::cout << "matrixScale:\n" << matrixScale << "\n";
    std::cout << "look_at:\n" << look_at << "\n";
    std::cout << "rotate:\n" << rotate << "\n";
    std::cout << "m_:\n" << m << "\n";
    std::cout << "inv_:\n" << inv << "\n";*/

    return {m, inv};
}

template<typename Value>
class aabb {
public:
    using Matrix = enoki::Matrix<Value, 4>;

    aabb() : m_(enoki::identity<Matrix>()), inv_(enoki::identity<Matrix>()) {}

    aabb(const point3<Value> &t, const vec3<Value> &s)
            : m_(enoki::translate<Matrix>(-t.data()) * enoki::scale<Matrix>(1 / s.data())),
              inv_(enoki::translate<Matrix>(t.data()) * enoki::scale<Matrix>(s.data())) {}

    point3<Value> min() const {
        return point3<Value>(inv_ * enoki::Array<Value, 4>{0, 0, 0, 1});
    }

    point3<Value> max() const {
        return point3<Value>(inv_ * enoki::Array<Value, 4>{1, 1, 1, 1});
    }

private:
    Matrix m_;  // World to local matrix
    Matrix inv_;// Local to World matrix
};

template<typename Value>
class rectangle {
public:
    using Matrix4x4 = enoki::Matrix<Value, 4>;
    using Matrix3x3 = enoki::Matrix<Value, 3>;

    using Vector3 = enoki::Array<Value, 3>;
    using Vector4 = enoki::Array<Value, 4>;

    using Scalar = enoki::entry_t<Matrix4x4>;
    using Radian = radian<Scalar>;
public:

    rectangle() : pixels_(1), m_(enoki::identity<Matrix4x4>()), inv_(enoki::identity<Matrix4x4>()),
                  normal_(calcNormal()) {}

    rectangle(const point2<Value> &pixels, Radian rotX, Radian rotY, Radian rotZ) : pixels_(
            pixels), m_(), inv_(), normal_(0) {
        auto[m, inv] = affine_transformation<Matrix4x4>(Vector3{0}, Vector3{1}, rotX, rotY, rotZ);

        m_ = m;
        inv_ = inv;
        normal_ = calcNormal();
    }

    rectangle(const point2<Value> &pixels, const vec3<Value> &translation, Radian rotX, Radian rotY, Radian rotZ)
            : pixels_(
            pixels), m_(), inv_(), normal_(0) {
        auto[m, inv] = affine_transformation<Matrix4x4>(translation.data(), Vector3{1}, rotX, rotY, rotZ);

        m_ = m;
        inv_ = inv;
        normal_ = calcNormal();
    }

    rectangle(const point2<Value> &pixels, const vec3<Value> &translation, const vec3<Value> &scale, Radian rotX,
              Radian rotY, Radian rotZ) : pixels_(
            pixels), m_(), inv_(), normal_(0) {
        auto[m, inv] = affine_transformation<Matrix4x4>(translation.data(), scale.data(), rotX, rotY, rotZ);

        m_ = m;
        inv_ = inv;
        normal_ = calcNormal();
    }

    rectangle(const point2<Value> &pixels, const vec3<Value> &translation, Scalar scale, Radian rotX, Radian rotY,
              Radian rotZ) : pixels_(
            pixels), m_(), inv_(), normal_(0) {
        auto[m, inv] = affine_transformation<Matrix4x4>(translation.data(), Vector3{scale}, rotX, rotY, rotZ);

        m_ = m;
        inv_ = inv;
        normal_ = calcNormal();
    }

    rectangle(const point2<Value> &pixels, const point2<Value> &t, const vec2<Value> &s) : pixels_(pixels), m_(),
                                                                                           inv_(), normal_(0) {
        /*enoki::Array<Value, 3> tmp_scale = enoki::concat(1 / s.data(), enoki::Array<Value, 1>{1});

        auto tmp_scale_2d = enoki::scale<Matrix3x3>(1 / s.data());
        auto tmp_translate_2d = enoki::translate<Matrix3x3>(enoki::Array<Value, 2>{0.5, 0.5} - t.data());

        std::cout << "Scale: " << tmp_scale_2d << "\n";
        std::cout << "translate: " << tmp_translate_2d << "\n";

        auto scale = Matrix4x4{tmp_scale_2d};

        m_ = Matrix4x4{tmp_translate_2d * tmp_scale_2d};
        inv_ = enoki::inverse(m_);

        std::cout << "m_:\n" << m_ << "\n";
        std::cout << "inv_:\n" << inv_ << "\n";*/

        /*auto scale_2d = enoki::scale<Matrix4x4>(tmp_scale);

        auto base = enoki::Array<Value, 3>{0.5, 0.5, 0};
        enoki::Array<Value, 3> concat = enoki::concat(t.data(), enoki::Array<Value, 1>{0});
        enoki::Array<Value, 3> tmp = base - concat;
        auto translate_2d = enoki::translate<Matrix4x4>(tmp);*/
        // auto translation_3d = enoki::identity<Matrix>();
        // auto rotate_3d = enoki::identity<Matrix>();

        /*
         * enoki::scale<Matrix>(1 / s.data()) *
                 (enoki::translate<Matrix>(enoki::Array<Value, 3>{0.5, 0.5, 0} - t.data())),
         * enoki::scale<Matrix>(s.data()) *
                   (enoki::translate<Matrix>(enoki::Array<Value, 3>{-0.5, -0.5, 0} + t.data()))
         */

        // First translate 2D, then scale in 2D, then move in 3D, then rotate in 3D
        // m_ =  translate_2d * scale_2d;

        normal_ = calcNormal();
    }

    point3<Value> coordFromLocal(const point2<Value> &p) const {
        enoki::Array<Value, 2> pixelCenter = (p.data() + point2<Value>{0.5, 0.5}.data()) / pixels_.data();
        return point3<Value>(m_ * enoki::concat(pixelCenter, enoki::Array<Value, 2>{0, 1}));
    }

    point3<Value> min() const {
        return point3<Value>(m_ * enoki::Array<Value, 4>{0, 0, 0, 1});
    }

    point3<Value> max() const {
        return point3<Value>(m_ * enoki::Array<Value, 4>{1, 1, 0, 1});
    }

    point3<Value> center() const {
        return point3<Value>(m_ * enoki::Array<Value, 4>{0.5, 0.5, 0, 1});
    }

    vec3<Value> normal() const {
        return normal_;
    }

private:
    vec3<Value> calcNormal() const {
        auto v1 = min();
        auto v2 = point3<Value>(m_ * enoki::Array<Value, 4>{1, 0, 0, 1});
        auto v3 = max();

        // take vector, which is perpendicular to the edge between v2, v3 and v2, v1
        auto n = cross(v3 - v2, v1 - v2);
        return normalize(n);
    }

    point2<Value> pixels_; // Number of pixels
    Matrix4x4 m_;  // Local to Global matrix
    Matrix4x4 inv_;// Global to Local matrix
    vec3<Value> normal_; // Normal to rectangle
};

template<typename Value>
class curved_rectangle {
public:
    using Matrix4x4 = enoki::Matrix<Value, 4>;
    using Matrix3x3 = enoki::Matrix<Value, 3>;

    using Vector3 = enoki::Array<Value, 3>;
    using Vector4 = enoki::Array<Value, 4>;

    using Scalar = enoki::entry_t<Matrix4x4>;
    using Radian = radian<Scalar>;
    using Angle = degree<Scalar>;
public:

    curved_rectangle() : pixels_(1), m_(enoki::identity<Matrix4x4>()), inv_(enoki::identity<Matrix4x4>()) {}

    /**
     * Assume curved rectangle has a center at the origin
     *
     * @param pixels
     * @param principalPoint
     * @param radius
     * @param theta
     */
    curved_rectangle(const point2<Value> &pixels, const point3<Value> &principalPoint, Scalar height_scale = Scalar{1},
                     Angle theta = degree<float>(180)) : pixels_(
            pixels), theta_(theta.get() / 2), m_(), inv_() {

        auto [m, inv] = cylindrical_transformation<Matrix4x4>(Vector3{0}, principalPoint.data(), height_scale);

        m_ = m;
        inv_ = inv;
    }

    curved_rectangle(const point2<Value> &pixels, const point3<Value> &center, const point3<Value> &principalPoint, Scalar height_scale = Scalar{1},
                     Angle theta = degree<float>(180)) : pixels_(
            pixels), theta_(theta.get() / 2), m_(), inv_() {

        auto [m, inv] = cylindrical_transformation<Matrix4x4>(center.data(), principalPoint.data(), 1);

        m_ = m;
        inv_ = inv;
    }


    point3<Value> coordFromLocal(const point2<Value> &p) const {
        // angle, to go from one pixel to the next
        auto step = (theta_.get() * 2) / pixels_.x();

        // z coord in cylindrical coordinates, such that 0 is top and 1 is bottom
        auto z = (1 - p.y()) - 0.5;

        // phi of cylindrical coordinate in radian
        auto phi = (p.x() * theta_.get() * 2) - theta_.get();
        auto phi_rad = phi / 180 * M_PI;

        // map cylindrical to cartesian coordinates (with r = 1)
        auto local = Vector4{
                enoki::cos(phi_rad),
                enoki::sin(phi_rad),
                z,
                1
        };

        /*std::cout << "Local: " << local << "\n";
        std::cout << "Global: " << m_ * local << "\n";*/

        // transfer local to global
        return point3<Value>(m_ * local);
    }

    point3<Value> principal_point() const {
        return point3<Value>(m_ * enoki::Array<Value, 4>{1, 0, 0, 1});
    }

    point3<Value> center() const {
        return point3<Value>(m_ * enoki::Array<Value, 4>{0, 0, 0, 1});
    }

private:

    point2<Value> pixels_; // Number of pixels
    Matrix4x4 m_;  // Local to Global matrix
    Matrix4x4 inv_;// Global to Local matrix
    Angle theta_; // angle of between line from center to principal point and one outer edge with z = 0
};