
/**
 *
 * \file test_vector.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"
#include "TomoSect/vector.hpp"

static_assert(std::is_same_v<vec2<pack2<float>>::Type, float>, "Expect \'Type\' to be float");

TEST_CASE("Testing Vector") {
    SUBCASE("Testing Construction") {
        SUBCASE("With a single value") {
            SUBCASE("2D Vector") {
                auto v = vec2<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
            }
            SUBCASE("3D Vector") {
                auto v = vec3<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
                CHECK(v.z() == 1);
            }
            SUBCASE("3D Vector") {
                auto v = vec4<float>(1);

                CHECK(v.x() == 1);
                CHECK(v.y() == 1);
                CHECK(v.z() == 1);
                CHECK(v.w() == 1);
            }
        }
        SUBCASE("With multiple values") {
            SUBCASE("2D Vector") {
                auto v = vec2<float>(1, 2);

                CHECK(v.x() == 1);
                CHECK(v.y() == 2);
            }
            SUBCASE("3D Vector") {
                auto v = vec3<float>(1, 2, 3);

                CHECK(v.x() == 1);
                CHECK(v.y() == 2);
                CHECK(v.z() == 3);
            }

            SUBCASE("4D Vector") {
                auto v = vec4<float>(1, 2, 3, 4);

                CHECK(v.x() == 1);
                CHECK(v.y() == 2);
                CHECK(v.z() == 3);
                CHECK(v.w() == 4);
            }
        }
        SUBCASE("With a pack") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(pack2<float>{1, 2});

                CHECK(v1.x() == pack2<float>{1, 2});
                CHECK(v1.y() == pack2<float>{1, 2});
            }
        }
        SUBCASE("With multiple packs") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(pack2<float>{1, 2}, pack2<float>{3, 4});

                CHECK(v1.x() == pack2<float>{1, 2});
                CHECK(v1.y() == pack2<float>{3, 4});
            }
        }
    }

    SUBCASE("Testing Math functions") {
        SUBCASE("Coefficient wise compound addition") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(2);
                auto v2 = vec2<pack2<float>>(pack2<float>{2, 3}, pack2<float>{4, 5});

                v1 += v2;

                CHECK(v1.x() == pack2<float>{4, 5});
                CHECK(v1.y() == pack2<float>{6, 7});

                v1 += pack2<float>{1, 1};

                CHECK(v1.x() == pack2<float>{5, 6});
                CHECK(v1.y() == pack2<float>{7, 8});

                v1 += 5;

                CHECK(v1.x() == pack2<float>{10, 11});
                CHECK(v1.y() == pack2<float>{12, 13});
            }
        }
        SUBCASE("Coefficient wise multiplication") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(2);
                auto v2 = vec2<pack2<float>>(pack2<float>{2, 3}, pack2<float>{4, 5});

                auto v = v1 * v2;

                CHECK(v.x() == pack2<float>{4, 6});
                CHECK(v.y() == pack2<float>{8, 10});
            }
        }
        SUBCASE("Coefficient wise addition") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(2);
                auto v2 = vec2<pack2<float>>(pack2<float>{2, 3}, pack2<float>{4, 5});

                auto v = v1 + v2;

                CHECK(v.x() == pack2<float>{4, 5});
                CHECK(v.y() == pack2<float>{6, 7});
            }
        }
        SUBCASE("Coefficient wise subtraction") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(2);
                auto v2 = vec2<pack2<float>>(pack2<float>{2, 3}, pack2<float>{4, 5});

                auto v = v1 - v2;

                CHECK(v.x() == pack2<float>{0, -1});
                CHECK(v.y() == pack2<float>{-2, -3});
            }
        }
        SUBCASE("Dot product") {
            SUBCASE("2D Vector") {
                auto v1 = vec2<pack2<float>>(2);
                auto v2 = vec2<pack2<float>>(pack2<float>{2, 3}, pack2<float>{4, 5});

                auto d = dot(v1, v2);

                static_assert(std::is_same_v<decltype(d), pack2<float>>, "Return type of dot product is a pack");

                CHECK(d == pack2<float>{12, 16});
            }
        }
    }
}