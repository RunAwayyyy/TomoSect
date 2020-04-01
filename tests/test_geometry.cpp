/**
 *
 * \file test_geometry.cpp
 *
 * \author David Frank (frankd@in.tum.de)
 */

#include "doctest.h"

#include "TomoSect/geometry.hpp"

TEST_CASE("Test ray") {
  SUBCASE("Concstruction") {
    auto p = point3<float>{1, 2, 3};
    auto v = vec3<float>{1, 1, 1};
    ray r = ray(p, v);

    auto origin = r.origin();

    CHECK(origin.x() == 1);
    CHECK(origin.y() == 2);
    CHECK(origin.z() == 3);

    auto direction = r.dir();

    CHECK(direction.x() == doctest::Approx(1 / std::sqrt(3)));
    CHECK(direction.y() == doctest::Approx(1 / std::sqrt(3)));
    CHECK(direction.z() == doctest::Approx(1 / std::sqrt(3)));
  }
}

TEST_CASE("Test AABB") {
  SUBCASE("Construction") {
    SUBCASE("At origin") {
      aabb box = aabb(point3<float>(0), vec3<float>{1, 1, 1});

      auto min = box.min();
      CHECK(min.x() == 0);
      CHECK(min.y() == 0);
      CHECK(min.z() == 0);

      auto max = box.max();
      CHECK(max.x() == 1);
      CHECK(max.y() == 1);
      CHECK(max.z() == 1);
    }
    SUBCASE("At origin with scale") {
      aabb box = aabb(point3<float>(0), vec3<float>{3, 2, 0.5});

      auto min = box.min();
      CHECK(min.x() == 0);
      CHECK(min.y() == 0);
      CHECK(min.z() == 0);

      auto max = box.max();
      CHECK(max.x() == 3);
      CHECK(max.y() == 2);
      CHECK(max.z() == 0.5);
    }
    SUBCASE("Shifted") {
      aabb box = aabb(point3<float>(-1, 3, 4), vec3<float>{1, 1, 1});

      auto min = box.min();
      CHECK(min.x() == -1);
      CHECK(min.y() == 3);
      CHECK(min.z() == 4);

      auto max = box.max();
      CHECK(max.x() == 0);
      CHECK(max.y() == 4);
      CHECK(max.z() == 5);
    }
  }
}

TEST_CASE("Testing degree and radian") {
  SUBCASE("Degree to radian") {
    SUBCASE("90°") {
      auto angle = degree<float>(90);

      CHECK(angle.to_radian() == doctest::Approx(M_PI_2));
    }
    SUBCASE("180°") {
      auto angle = degree<float>(180);

      CHECK(angle.to_radian() == doctest::Approx(M_PI));
    }
    SUBCASE("270°") {
      auto angle = degree<float>(270);

      CHECK(angle.to_radian() == doctest::Approx(1.5 * M_PI));
      CHECK(radian<float>(angle).get() == doctest::Approx(1.5 * M_PI));
    }
    SUBCASE("360°") {
      auto angle = degree<float>(360);

      CHECK(angle.to_radian() == doctest::Approx(2 * M_PI));
      CHECK(radian<float>(angle).get() == doctest::Approx(2 * M_PI));
    }
  }
  SUBCASE("Radian to degree") {
    SUBCASE("Pi / 2") {
      auto rad = radian<float>(M_PI_2);

      CHECK(rad.to_degree() == doctest::Approx(90));
      CHECK(degree<float>(rad).get() == doctest::Approx(90));
    }
    SUBCASE("Pi") {
      auto rad = radian<float>(M_PI);

      CHECK(rad.to_degree() == doctest::Approx(180));
      CHECK(degree<float>(rad).get() == doctest::Approx(180));
    }
  }
}

TEST_CASE("Test Rectangle") {
  SUBCASE("Construction") {
    SUBCASE("At origin") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), Angle(0), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-0.5));
      CHECK(min.y() == doctest::Approx(-0.5));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(0.5));
      CHECK(max.y() == doctest::Approx(0.5));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE("At origin, rotate 90 degree around z-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), Angle(0), Angle(0), Angle(90));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-0.5));
      CHECK(min.y() == doctest::Approx(0.5));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(0.5));
      CHECK(max.y() == doctest::Approx(-0.5));
      CHECK(max.z() == doctest::Approx(0));

      auto c = rect.center();
      CHECK(c.x() == doctest::Approx(0));
      CHECK(c.y() == doctest::Approx(0));
      CHECK(c.z() == doctest::Approx(0));
    }
    SUBCASE("At origin, rotate -90 degree around z-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0, 0, 0), Angle(0),
                            Angle(0), Angle(-90));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0.5));
      CHECK(min.y() == doctest::Approx(-0.5));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(-0.5));
      CHECK(max.y() == doctest::Approx(0.5));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE("At origin, rotate 90 degree around x-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0, 0, 0), Angle(90),
                            Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-0.5));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0.5));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(0.5));
      CHECK(max.y() == doctest::Approx(0));
      CHECK(max.z() == doctest::Approx(-0.5));
    }
    SUBCASE("At origin, rotate -90 degree around x-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0, 0, 0), Angle(-90),
                            Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-0.5));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(-0.5));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(0.5));
      CHECK(max.y() == doctest::Approx(0));
      CHECK(max.z() == doctest::Approx(0.5));
    }

    SUBCASE("Shifted by 0.5 in x and y direction") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0.5, 0.5, 0),
                            Angle(0), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(1));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE(
        "Shifted by 0.5 in x and y direction, rotate 90 degree around z-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0.5, 0.5, 0),
                            Angle(0), Angle(0), Angle(90));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(-1));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE("Shifted by 0.5 in x and y direction, rotate -90 degree around "
            "z-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0.5, 0.5, 0),
                            Angle(0), Angle(0), Angle(-90));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(-1));
      CHECK(max.y() == doctest::Approx(1));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE(
        "Shifted by 0.5 in x and y direction, rotate 90 degree around x-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0.5, 0.5, 0),
                            Angle(90), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(0));
      CHECK(max.z() == doctest::Approx(-1));
    }
    SUBCASE("Shifted by 0.5 in x and y direction, rotate -90 degree around "
            "x-axis") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0.5, 0.5, 0),
                            Angle(-90), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(0));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(0));
      CHECK(max.z() == doctest::Approx(1));
    }

    SUBCASE("At origin with scale") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0, 0, 0), 2,
                            Angle(0), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-1));
      CHECK(min.y() == doctest::Approx(-1));
      CHECK(min.z() == doctest::Approx(0));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(1));
      CHECK(max.z() == doctest::Approx(0));
    }
    SUBCASE("At origin with scale, turned around x-axis by -90°") {
      using Angle = degree<float>;
      auto rect = rectangle(point2<float>(10), vec3<float>(0, 0, 0), 2,
                            Angle(-90), Angle(0), Angle(0));

      auto min = rect.min();
      CHECK(min.x() == doctest::Approx(-1));
      CHECK(min.y() == doctest::Approx(0));
      CHECK(min.z() == doctest::Approx(-1));

      auto max = rect.max();
      CHECK(max.x() == doctest::Approx(1));
      CHECK(max.y() == doctest::Approx(0));
      CHECK(max.z() == doctest::Approx(1));
    }
    //        SUBCASE("At origin with scale") {
    //            rectangle rect = rectangle(point2<float>(10),
    //            point2<float>(0), vec2<float>{3, 2});
    //
    //            auto min = rect.min();
    //            CHECK(min.x() == -1.5);
    //            CHECK(min.y() == -1);
    //            CHECK(min.z() == 0);
    //
    //            auto max = rect.max();
    //            CHECK(max.x() == 1.5);
    //            CHECK(max.y() == 1);
    //            CHECK(max.z() == 0);
    //        }
    //        SUBCASE("Shifted") {
    //            rectangle rect = rectangle(point2<float>(10),point2<float>(-1,
    //            3), vec2<float>{1, 1});
    //
    //            auto min = rect.min();
    //            CHECK(min.x() == -1.5);
    //            CHECK(min.y() == 2.5);
    //            CHECK(min.z() == 0);
    //
    //            auto max = rect.max();
    //            CHECK(max.x() == -0.5);
    //            CHECK(max.y() == 3.5);
    //            CHECK(max.z() == 0);
    //        }
  }
  //    SUBCASE("Image pixels") {
  //        SUBCASE("With 10 pixels centered at origin") {
  //            rectangle rect = rectangle(point2<float>(10),point2<float>(0),
  //            vec2<float>{1, 1});
  //
  //            for(int x=0; x<10; x++) {
  //                for(int y=0; y<10; y++) {
  //                    auto p = rect.coordFromLocal(point2<float>{x, y});
  //
  //                    float xf = static_cast<float>(x) - 5;
  //                    float yf = static_cast<float>(y) - 5;
  //
  //                    CHECK(p.x() == doctest::Approx(xf / 10 + 0.05));
  //                    CHECK(p.y() == doctest::Approx(yf / 10 + 0.05));
  //                    CHECK(p.z() == 0);
  //                }
  //            }
  //        }
  //    }
}

TEST_CASE("Test Curved Rectangle") {
  SUBCASE("Construction") {
    SUBCASE("At origin") {
      using Angle = degree<float>;
      auto rect = curved_rectangle(point2<float>(1), point3<float>(0, 0, -1));

      auto p = rect.principal_point();
      CHECK(p.x() == doctest::Approx(0));
      CHECK(p.y() == doctest::Approx(0));
      CHECK(p.z() == doctest::Approx(-1));

      auto c = rect.center();
      CHECK(c.x() == doctest::Approx(0));
      CHECK(c.y() == doctest::Approx(0));
      CHECK(c.z() == doctest::Approx(0));

      auto top_left = rect.coordFromLocal(point2<float>{0, 0});
      CHECK(top_left.x() == doctest::Approx(-1));
      CHECK(top_left.y() == doctest::Approx(0.5));
      CHECK(top_left.z() == doctest::Approx(0));

      auto lower_right = rect.coordFromLocal(point2<float>{1, 1});
      CHECK(lower_right.x() == doctest::Approx(1));
      CHECK(lower_right.y() == doctest::Approx(-0.5));
      CHECK(lower_right.z() == doctest::Approx(0));

      auto center_45_degree = rect.coordFromLocal(point2<float>{0.25, 0.5});
      CHECK(center_45_degree.x() ==
            doctest::Approx(-std::cos(degree<float>(45).to_radian())));
      CHECK(center_45_degree.y() == doctest::Approx(0));
      CHECK(center_45_degree.z() ==
            doctest::Approx(-std::cos(degree<float>(45).to_radian())));

      auto center_135_degree = rect.coordFromLocal(point2<float>{0.75, 0.5});
      CHECK(center_135_degree.x() ==
            doctest::Approx(std::cos(degree<float>(45).to_radian())));
      CHECK(center_135_degree.y() == doctest::Approx(0));
      CHECK(center_135_degree.z() ==
            doctest::Approx(-std::cos(degree<float>(45).to_radian())));

      auto principal_point = rect.coordFromLocal(point2<float>{0.5, 0.5});
      CHECK(p.data() == principal_point.data());
    }

    SUBCASE("At origin, radius = 2") {
      using Angle = degree<float>;
      auto rect =
          curved_rectangle(point2<float>(1), point3<float>(0, 0, -2), 6);

      auto p = rect.principal_point();
      CHECK(p.x() == doctest::Approx(0));
      CHECK(p.y() == doctest::Approx(0));
      CHECK(p.z() == doctest::Approx(-2));

      auto c = rect.center();
      CHECK(c.x() == doctest::Approx(0));
      CHECK(c.y() == doctest::Approx(0));
      CHECK(c.z() == doctest::Approx(0));

      auto top_left = rect.coordFromLocal(point2<float>{0, 0});
      CHECK(top_left.x() == doctest::Approx(-2));
      CHECK(top_left.y() == doctest::Approx(0.5 * 6));
      CHECK(top_left.z() == doctest::Approx(0));

      auto lower_right = rect.coordFromLocal(point2<float>{1, 1});
      CHECK(lower_right.x() == doctest::Approx(2));
      CHECK(lower_right.y() == doctest::Approx(-0.5 * 6));
      CHECK(lower_right.z() == doctest::Approx(0));

      auto principal_point = rect.coordFromLocal(point2<float>{0.5, 0.5});
      CHECK(p.data() == principal_point.data());
    }
    SUBCASE("Translated") {
      using Angle = degree<float>;
      auto rect = curved_rectangle(point2<float>(10), point3<float>(1, 1, 0),
                                   point3<float>(1, 1, -1));

      auto p = rect.principal_point();
      CHECK(p.x() == doctest::Approx(1));
      CHECK(p.y() == doctest::Approx(1));
      CHECK(p.z() == doctest::Approx(-1));

      auto c = rect.center();
      CHECK(c.x() == doctest::Approx(1));
      CHECK(c.y() == doctest::Approx(1));
      CHECK(c.z() == doctest::Approx(0));

      auto top_left = rect.coordFromLocal(point2<float>{0, 0});
      CHECK(top_left.x() == doctest::Approx(0));
      CHECK(top_left.y() == doctest::Approx(1.5));
      CHECK(top_left.z() == doctest::Approx(0));

      auto lower_right = rect.coordFromLocal(point2<float>{1, 1});
      CHECK(lower_right.x() == doctest::Approx(2));
      CHECK(lower_right.y() == doctest::Approx(0.5));
      CHECK(lower_right.z() == doctest::Approx(0));

      auto principal_point = rect.coordFromLocal(point2<float>{0.5, 0.5});
      CHECK(p.data() == principal_point.data());
    }
  }
}