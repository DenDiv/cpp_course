#pragma once
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

constexpr float flt_tolerance = 0.00001;

namespace geom_prim
{
    struct obj
    {
        virtual void print() const = 0;
        virtual bool valid() const = 0;
    };
    //----------------------------------------------------------------------------
    //
    // point_t -- basic (x, y, z) point
    //
    //----------------------------------------------------------------------------
    struct point_t : public obj
    {
        point_t(float x_c, float y_c, float z_c) : x(x_c), y(y_c), z(z_c) {}

        void print() const { std::cout << "(" << x << " ; " << y << " ; " << z << ")"; }

        bool valid() const { return !(x != x || y != y || z != z); }

        bool equal(const point_t &rhs) const
        {
            assert(valid() && rhs.valid());
            return (std::abs(x - rhs.x) < flt_tolerance) &&
                   (std::abs(y - rhs.y) < flt_tolerance) &&
                   (std::abs(z - rhs.z) < flt_tolerance);
        }

        float x = NAN;
        float y = NAN;
        float z = NAN;
    };

    //----------------------------------------------------------------------------
    //
    // vec_t -- basic (x, y, z) vector from p1 to p2 points
    //
    //----------------------------------------------------------------------------
    struct vec_t : public obj
    {
        vec_t(const point_t &p1, const point_t &p2)
        {
            if (p1.equal(p2))
                throw std::runtime_error("p1 is equal to p2");

            x = p2.x - p1.x;
            y = p2.y - p1.y;
            z = p2.z - p1.z;
        }

        vec_t(float x_c, float y_c, float z_c) : x(x_c), y(y_c), z(z_c) {}

        void print() const { std::cout << "(" << x << " ; " << y << " ; " << z << ")"; }

        bool valid() const { return !(x != x || y != y || z != z); }

        bool equal(const vec_t &rhs) const
        {
            assert(valid() && rhs.valid());
            return (std::abs(x - rhs.x) < flt_tolerance) &&
                   (std::abs(y - rhs.y) < flt_tolerance) &&
                   (std::abs(z - rhs.z) < flt_tolerance);
        }

        float x = NAN;
        float y = NAN;
        float z = NAN;
    };

    // det_2d - computes determinant for 2d matrix
    // matrix form:
    // | a b |
    // | c d |
    float det_2d(float a, float b, float c, float d)
    {
        return a * d - b * c;
    }

    // vec multiplication
    vec_t vec_mul(const vec_t &vec1, const vec_t &vec2)
    {
        float x_res = det_2d(vec1.y, vec1.z, vec2.y, vec2.z);
        float y_res = -1 * det_2d(vec1.x, vec1.z, vec2.x, vec2.z);
        float z_res = det_2d(vec1.x, vec1.y, vec2.x, vec2.y);
        return vec_t(x_res, y_res, z_res);
    }

    //----------------------------------------------------------------------------
    //
    // plane_t -- plane in form of ax + by + cz + d = 0
    //
    //----------------------------------------------------------------------------
    struct plane_t : public obj
    {
        plane_t(const point_t &p1, const point_t &p2, const point_t &p3)
        {
            if (p1.equal(p2) || p1.equal(p3) || p2.equal(p3))
                throw std::runtime_error("some points are equal");

            vec_t vec1(p1, p2), vec2(p1, p3);
            vec_t vec_mul_res = vec_mul(vec1, vec2);
            a = vec_mul_res.x;
            b = vec_mul_res.y;
            c = vec_mul_res.z;
            d = -1 * (a * p1.x + b * p1.y + c * p1.z);
        }

        void print() const { std::cout << "(" << a << " ; " << b << " ; " << c << " ; " << d << ")"; }

        bool valid() const { return !(a != a || b != b || c != c || d != d); }

        bool equal(const plane_t &rhs) const
        {
            assert(valid() && rhs.valid());
            return (std::abs(a - rhs.a) < flt_tolerance) &&
                   (std::abs(b - rhs.b) < flt_tolerance) &&
                   (std::abs(c - rhs.c) < flt_tolerance) &&
                   (std::abs(d - rhs.d) < flt_tolerance);
        }

        float a = -1.0f;
        float b = 1.0f;
        float c = 0.0f;
        float d = 0.0f;
    };

    //----------------------------------------------------------------------------
    //
    // triangle_t -- triangle struct
    //
    //----------------------------------------------------------------------------
    struct triangle_t : public obj
    {
        triangle_t(const point_t &tr_p1, const point_t &tr_p2, const point_t &tr_p3) : p1(tr_p1), p2(tr_p2), p3(tr_p3), pl(plane_t(p1, p2, p3)) {}

        void print() const
        {
            std::cout << "p1: ";
            p1.print();
            std::cout << std::endl;

            std::cout << "p2: ";
            p2.print();
            std::cout << std::endl;

            std::cout << "p3: ";
            p3.print();
            std::cout << std::endl;

            std::cout << "plane coefs: ";
            pl.print();
            std::cout << std::endl;
        }

        bool valid() const
        {
            return pl.valid();
        }

        bool equal(const triangle_t &rhs) const
        {
            assert(valid() && rhs.valid());
            if (!(p1.equal(rhs.p1) || p1.equal(rhs.p2) || p1.equal(rhs.p3)))
                return false;
            if (!(p2.equal(rhs.p1) || p2.equal(rhs.p2) || p2.equal(rhs.p3)))
                return false;
            if (!(p3.equal(rhs.p1) || p3.equal(rhs.p2) || p3.equal(rhs.p3)))
                return false;
            return true;
        }

        point_t p1, p2, p3;
        plane_t pl;
    };

    // signed distance from point to plane for triangle intersection task
    float s_dist_point2plane(const point_t &p1, const plane_t &pl)
    {
        return pl.a * p1.x + pl.b * p1.y + pl.c * p1.z + pl.d;
    }

    // computes distances for array of points
    std::vector<float> get_s_dist_for_arr(const std::vector<point_t> &points_vec, const plane_t &pl)
    {
        std::vector<float> res;
        for (const auto &p : points_vec)
        {
            res.push_back(s_dist_point2plane(p, pl));
        }
        return res;
    }

    // simple check for triangle and another plane intersection
    bool simple_tr_int_check(const std::vector<float> &dist_arr)
    {
        assert(dist_arr.size() == 3);
        if (((dist_arr[0] > 0) && (dist_arr[1] > 0) && (dist_arr[2] > 0)) || ((dist_arr[0] < 0) && (dist_arr[1] < 0) && (dist_arr[2] < 0)))
            return false;
        return true;
    }

    // checks that triangles in one plane
    bool tr_in_one_pl_check(const std::vector<float> &dist_arr)
    {
        assert(dist_arr.size() == 3);
        if (std::abs(dist_arr[0]) < flt_tolerance && std::abs(dist_arr[1]) < flt_tolerance && std::abs(dist_arr[2]) < flt_tolerance)
            return true;
        return false;
    }

    // swaps distances and points that v1 and v3 are on one side of plane
    void swap_d_p(std::vector<point_t> &p_arr, std::vector<float> &d_arr)
    {
        if (d_arr[0] * d_arr[1] > 0)
        {
            std::iter_swap(d_arr.begin() + 1, d_arr.begin() + 2);
            std::iter_swap(p_arr.begin() + 1, p_arr.begin() + 2);
        }
        else if (d_arr[1] * d_arr[2] > 0)
        {
            std::iter_swap(d_arr.begin(), d_arr.begin() + 1);
            std::iter_swap(p_arr.begin(), p_arr.begin() + 1);
        }
    }

    // finds random point O on the intersection of planes
    point_t find_O(const plane_t &pl1, const plane_t &pl2)
    {
        float Ox, Oy, Oz;
        float determ_1 = det_2d(pl1.a, pl1.b, pl2.a, pl2.b);
        float determ_2 = det_2d(pl1.a, pl1.c, pl2.a, pl2.c);
        float determ_3 = det_2d(pl1.b, pl1.c, pl2.b, pl2.c);

        // if determ_1 != 0, let z = 0
        if (std::abs(determ_1) > flt_tolerance)
        {
            Ox = det_2d(-pl1.d, pl1.b, -pl2.d, pl2.b) / determ_1;
            Oy = det_2d(pl1.a, -pl1.d, pl2.a, -pl2.d) / determ_1;
            Oz = 0.0f;
        }
        // if determ_1 == 0 and determ_2 != 0, let y = 0
        else if (std::abs(determ_2) > flt_tolerance)
        {
            Ox = det_2d(-pl1.d, pl1.c, -pl2.d, pl2.c) / determ_2;
            Oy = 0.0f;
            Oz = det_2d(pl1.a, -pl1.d, pl2.a, -pl2.d) / determ_2;
        }
        // if determ_1 == 0 and determ_2 == 0 and determ_3 != 0, let x = 0
        else if (std::abs(determ_3) > flt_tolerance)
        {
            Ox = 0.0f;
            Oy = det_2d(-pl1.d, pl1.c, -pl2.d, pl2.c) / determ_3;
            Oz = det_2d(pl1.b, -pl1.d, pl2.b, -pl2.d) / determ_3;
        }

        return point_t(Ox, Oy, Oz);
    }

    // finds projection of triangle points to L
    std::vector<float> find_proj(const std::vector<point_t> &p_arr, const vec_t &D, const point_t &O)
    {
        std::vector<float> proj_arr;
        for (auto &p : p_arr)
        {
            // TODO: if points O and p are equal code breaks
            vec_t diff_vec(O, p);
            float proj_val = diff_vec.x * D.x + diff_vec.y * D.y + diff_vec.z * D.z;
            proj_arr.push_back(proj_val);
        }
        return proj_arr;
    }

    // finds triangle and plane (from another triangle) intersection boards
    std::vector<float> find_inters_board(const std::vector<float> &proj_arr, const std::vector<float> &d_arr)
    {
        // assume that p1 and p3 are on the single side
        float t1, t2;
        t1 = proj_arr[0] + (proj_arr[1] - proj_arr[0]) * d_arr[0] / (d_arr[0] - d_arr[1]);
        t2 = proj_arr[2] + (proj_arr[1] - proj_arr[2]) * d_arr[2] / (d_arr[2] - d_arr[1]);

        if (t1 > t2)
            std::swap(t1, t2);

        return std::vector<float>{t1, t2};
    }

    // finally checks intersection
    bool check_inters(const std::vector<float> &inters_b_1, const std::vector<float> &inters_b_2)
    {
        float x1, x2, y1, y2;
        x1 = inters_b_1[0];
        x2 = inters_b_1[1];
        y1 = inters_b_2[0];
        y2 = inters_b_2[1];

        if ((x1 >= y1) && (y2 > x1))
            return true;
        else if ((x1 < y1) && (y1 < x2))
            return true;
        else
            return false;
    }

    // function that checkes if triangles intersection
    // according to this article:
    // http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf
    bool tr_inters(const triangle_t &tr1, const triangle_t &tr2)
    {
        // 1. triangles are equal
        if (tr1.equal(tr2))
            return true;

        // 2. if all points of T1 lie on one side of plane2
        //    or all points of T2 lie on one side of plane1
        std::vector<point_t> tr1_p_arr{tr1.p1, tr1.p2, tr1.p3};
        std::vector<point_t> tr2_p_arr{tr2.p1, tr2.p2, tr2.p3};
        std::vector<float> d_arr_from_tr1 = get_s_dist_for_arr(tr1_p_arr, tr2.pl);
        std::vector<float> d_arr_from_tr2 = get_s_dist_for_arr(tr2_p_arr, tr1.pl);

        if (!simple_tr_int_check(d_arr_from_tr1))
            return false;

        if (!simple_tr_int_check(d_arr_from_tr2))
            return false;

        // TODO: case when triangles on one plane
        if (tr_in_one_pl_check(d_arr_from_tr1) || tr_in_one_pl_check(d_arr_from_tr2))
        {
            throw std::runtime_error("currently case when triangles on one plane not supported. TODO.");
        }

        // 3. if points of T1 lie on different sides of plane2
        // firstly find 2 out of 3 vertices of T1 that lie on the one side of plane2
        // there are v1 and v3
        swap_d_p(tr1_p_arr, d_arr_from_tr1);
        swap_d_p(tr2_p_arr, d_arr_from_tr2);

        // find random O point that belongs to L (inters of plane1 and plane2)
        point_t O = find_O(tr1.pl, tr2.pl);
        vec_t N1(tr1.pl.a, tr1.pl.b, tr1.pl.c), N2(tr2.pl.a, tr2.pl.b, tr2.pl.c);
        vec_t D = vec_mul(N1, N2);

        // find projections of triangles to L
        std::vector<float> proj_t1 = find_proj(tr1_p_arr, D, O);
        std::vector<float> proj_t2 = find_proj(tr2_p_arr, D, O);

        // find intersection boards
        std::vector<float> inters_board_tr_1 = find_inters_board(proj_t1, d_arr_from_tr1);
        std::vector<float> inters_board_tr_2 = find_inters_board(proj_t2, d_arr_from_tr2);

        return check_inters(inters_board_tr_1, inters_board_tr_2);
    }

}