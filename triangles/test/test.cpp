#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

#include <primitives.h>
#define eps 0.000001

using namespace geom_prim;

TEST(PrimitivesTest, vec_test)
{
    point_t p1(1.25f, 1.0f, 2.25f), p2(5.0f, 1.25f, 0.0f);
    vec_t vec(p1, p2);
    ASSERT_TRUE(vec.x - 3.75f < eps);
    ASSERT_TRUE(vec.y - 0.25f < eps);
    ASSERT_TRUE(vec.z - (-2.25f) < eps);

    ASSERT_TRUE(vec.valid());
}

TEST(PrimitivesTest, vec_mul_test)
{
    vec_t vec1(1.5f, 1.0f, 5.0f), vec2(3.6f, 2.5f, 0.3f);
    vec_t res_vec = vec_mul(vec1, vec2);
    ASSERT_TRUE(res_vec.x - (-12.2f) < eps);
    ASSERT_TRUE(res_vec.y - 17.55f < eps);
    ASSERT_TRUE(res_vec.z - 0.15f < eps);

    ASSERT_TRUE(res_vec.valid());
}

TEST(PrimitivesTest, plane_test)
{
    point_t p1(1.25f, 1.0f, 2.25f), p2(5.0f, 1.25f, 0.0f), p3(2.1f, 4.1f, 9.8f);
    plane_t pl(p1, p2, p3);
    ASSERT_TRUE(pl.a - 8.8625f < eps);
    ASSERT_TRUE(pl.b - (-30.225f) < eps);
    ASSERT_TRUE(pl.c - 11.4125f < eps);
    ASSERT_TRUE(pl.d - (-6.53125f) < eps);

    ASSERT_TRUE(pl.valid());

    point_t p4(6.5f, 4.4f, 7.6f);
    float dist = pl.s_dist_from_p(p4);
    dist /= std::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
    ASSERT_TRUE(dist - 0.14387484806520984 < eps);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}