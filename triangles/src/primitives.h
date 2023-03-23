#pragma once
#include <iostream>
#include <cassert>
#include <cmath>

#define flt_tolerance 0.00001

namespace geom_prim
{
    //----------------------------------------------------------------------------
    //
    // point_t -- basic (x, y, z) point
    //
    //----------------------------------------------------------------------------
    struct point_t
    {
        float x = NAN, y = NAN, z = NAN;

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
    };

    //----------------------------------------------------------------------------
    //
    // vec_t -- basic (x, y, z) vector from p1 to p2 points
    //
    //----------------------------------------------------------------------------
    struct vec_t
    {
        float x = NAN, y = NAN, z = NAN;

        vec_t(const point_t &p1, const point_t &p2)
        {
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
    };

    // vec multiplication
    vec_t vec_mul(const vec_t &vec1, const vec_t &vec2)
    {
        float x_res = vec1.y * vec2.z - vec1.z * vec2.y;
        float y_res = -1 * (vec1.x * vec2.z - vec1.z * vec2.x);
        float z_res = vec1.x * vec2.y - vec1.y * vec2.x;
        return vec_t(x_res, y_res, z_res);
    };

    //----------------------------------------------------------------------------
    //
    // plane_t -- plane in form of ax + by + cz + d = 0
    //
    //----------------------------------------------------------------------------
    struct plane_t
    {
        float a = -1.0f, b = 1.0f, c = 0.0f, d = 0.0f;

        plane_t(point_t p1, point_t p2, point_t p3)
        {
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

        float s_dist_from_p(const point_t &p1) const
        {
            return a * p1.x + b * p1.y + c * p1.z + d;
        }
    };
}