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
    ASSERT_TRUE(std::abs(vec.x - 3.75f) < eps);
    ASSERT_TRUE(std::abs(vec.y - 0.25f) < eps);
    ASSERT_TRUE(std::abs(vec.z - (-2.25f)) < eps);

    EXPECT_THROW(vec_t(p1, p1), std::runtime_error);
}

TEST(PrimitivesTest, vec_mul_test)
{
    vec_t vec1(1.5f, 1.0f, 5.0f), vec2(3.6f, 2.5f, 0.3f);
    vec_t res_vec = vec_mul(vec1, vec2);
    ASSERT_TRUE(std::abs(res_vec.x - (-12.2f)) < eps);
    ASSERT_TRUE(std::abs(res_vec.y - 17.55f) < eps);
    ASSERT_TRUE(std::abs(res_vec.z - 0.15f) < eps);

    ASSERT_TRUE(res_vec.valid());
}

TEST(PrimitivesTest, plane_test)
{
    point_t p1(1.25f, 1.0f, 2.25f), p2(5.0f, 1.25f, 0.0f), p3(2.1f, 4.1f, 9.8f);
    plane_t pl(p1, p2, p3);
    ASSERT_TRUE(std::abs(pl.a - 8.8625f) < eps);
    ASSERT_TRUE(std::abs(pl.b - (-30.225f)) < eps);
    ASSERT_TRUE(std::abs(pl.c - 11.4125f) < eps);
    ASSERT_TRUE(std::abs(pl.d - (-6.53125f)) < eps);

    ASSERT_TRUE(pl.valid());
    EXPECT_THROW(plane_t pl(p1, p2, p1), std::runtime_error);

    point_t p4(6.5f, 4.4f, 7.6f);
    float dist = s_dist_point2plane(p4, pl);
    dist /= std::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
    ASSERT_TRUE(std::abs(dist - 0.14387484806520984) < eps);
}

TEST(PrimitivesTest, triangle_test)
{
    point_t p1(1.25f, 1.0f, 2.25f), p2(5.0f, 1.25f, 0.0f), p3(2.1f, 4.1f, 9.8f);
    triangle_t tr(p1, p2, p3);

    ASSERT_TRUE(tr.valid());

    triangle_t tr1(p2, p1, p3);
    triangle_t tr2(p1, p3, p2);
    triangle_t tr3(p3, p1, p2);
    ASSERT_TRUE(tr.equal(tr1));
    ASSERT_TRUE(tr.equal(tr2));
    ASSERT_TRUE(tr.equal(tr3));
}

TEST(PrimitivesTest, triangle_inters_test_main_func)
{
    point_t t1p1(0.0f, 0.0f, 0.0f), t1p2(5.0f, 0.0f, 5.0f), t1p3(0.0f, 5.0f, 5.0f);
    triangle_t tr1(t1p1, t1p2, t1p3);

    point_t t2p1(0.0f, 0.0f, 5.0f), t2p2(5.0f, 0.0f, 0.0f), t2p3(0.0f, 5.0f, 0.0f);
    triangle_t tr2(t2p1, t2p2, t2p3);

    std::vector<point_t> tr1_p_arr{tr1.p1, tr1.p2, tr1.p3};
    std::vector<point_t> tr2_p_arr{tr2.p1, tr2.p2, tr2.p3};
    std::vector<float> d_arr_from_tr1 = get_s_dist_for_arr(tr1_p_arr, tr2.pl);
    std::vector<float> d_arr_from_tr2 = get_s_dist_for_arr(tr2_p_arr, tr1.pl);

    // check swap func
    std::vector<point_t> tr1_p_arr_ans{t1p2, t1p1, t1p3};
    std::vector<point_t> tr2_p_arr_ans{t2p2, t2p1, t2p3};
    swap_d_p(tr1_p_arr, d_arr_from_tr1);
    swap_d_p(tr2_p_arr, d_arr_from_tr2);
    ASSERT_TRUE(tr1_p_arr[0].equal(tr1_p_arr_ans[0]) && tr1_p_arr[1].equal(tr1_p_arr_ans[1]) && tr1_p_arr[2].equal(tr1_p_arr_ans[2]));
    ASSERT_TRUE(tr2_p_arr[0].equal(tr2_p_arr_ans[0]) && tr2_p_arr[1].equal(tr2_p_arr_ans[1]) && tr2_p_arr[2].equal(tr2_p_arr_ans[2]));

    // check find O correctness
    point_t O = find_O(tr1.pl, tr2.pl);
    ASSERT_TRUE(std::abs(O.x * tr1.pl.a + O.y * tr1.pl.b + O.z * tr1.pl.c + tr1.pl.d) < eps);
    ASSERT_TRUE(std::abs(O.x * tr2.pl.a + O.y * tr2.pl.b + O.z * tr2.pl.c + tr2.pl.d) < eps);

    vec_t N1(tr1.pl.a, tr1.pl.b, tr1.pl.c), N2(tr2.pl.a, tr2.pl.b, tr2.pl.c);
    vec_t D = vec_mul(N1, N2);
    vec_t D_true(-1250.0f, 1250.0f, 0.0f);
    ASSERT_TRUE((std::abs(D.x - D_true.x) < eps) && (std::abs(D.y - D_true.y) < eps) && (std::abs(D.z - D_true.z) < eps));

    // check finally
    ASSERT_TRUE(tr_inters(tr1, tr2));
}

TEST(PrimitivesTest, triangle_inters_test_cases)
{
    point_t t1p1(0.0f, 0.0f, 0.0f), t1p2(1.0f, 0.0f, 1.0f), t1p3(0.0f, 1.0f, 1.0f);
    triangle_t tr1(t1p1, t1p2, t1p3);

    point_t t2p1(0.0f, 0.0f, 0.5f), t2p2(1.0f, 0.0f, 0.0f), t2p3(0.0f, 0.1f, 0.0f);
    triangle_t tr2(t2p1, t2p2, t2p3);

    point_t t3p1(-1.0f, -1.0f, -1.0f), t3p2(-2.0f, -3.0f, -0.5f), t3p3(-4.0f, -6.0f, -4.0f);
    triangle_t tr3(t3p1, t3p2, t3p3);

    point_t t4p1(10.0f, 10.0f, 10.0f), t4p2(15.0f, 12.0f, 13.5f), t4p3(20.0f, 10.0f, 15.0f);
    triangle_t tr4(t4p1, t4p2, t4p3);

    point_t t5p1(0.1f, 0.5f, 0.1f), t5p2(0.0f, 0.0f, 1.0f), t5p3(-1.0f, -1.0f, -1.0f);
    triangle_t tr5(t5p1, t5p2, t5p3);

    ASSERT_TRUE(tr_inters(tr1, tr2));
    ASSERT_TRUE(!tr_inters(tr1, tr3));
    ASSERT_TRUE(!tr_inters(tr2, tr3));
    ASSERT_TRUE(!tr_inters(tr1, tr4));
    ASSERT_TRUE(!tr_inters(tr2, tr4));
    ASSERT_TRUE(tr_inters(tr1, tr5));
}

TEST(PrimitivesTest, triangle_inters_error_cases)
{
    // this cases must be fixed in feature
    point_t t1p1(0.0f, 0.0f, 0.0f), t1p2(1.0f, 0.0f, 1.0f), t1p3(0.0f, 1.0f, 1.0f);
    triangle_t tr1(t1p1, t1p2, t1p3);

    point_t t2p1(0.1f, 0.1f, 0.1f), t2p2(0.0f, 0.0f, 1.0f), t2p3(-1.0f, -1.0f, -1.0f);
    triangle_t tr2(t2p1, t2p2, t2p3);

    ASSERT_TRUE(tr_inters(tr1, tr2));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}