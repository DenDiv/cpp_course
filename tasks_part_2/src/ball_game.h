#include <algorithm>
#include <iostream>
#include <cassert>
#include <math.h>
#include <mutex>

class Ball final
{
private:
    double x_;
    double y_;
    double xv_;
    double yv_;
    double t_;
    std::mutex mt_;
public:
    Ball(double x, double y, double xv, double yv, double t = 0.0): x_(x), y_(y), xv_(xv), yv_(yv), t_(t) {}

    void fly(double t)
    {
        assert(t > 0);
        std::lock_guard<std::mutex> lg(mt_); 
        x_ += xv_ * t;
        y_ += yv_ * t;
        t_ += t;
    }

    void push(double a_v, double a_alpha)
    {
        assert(a_v > 0);
        std::lock_guard<std::mutex> lg(mt_); 
        xv_ += a_v * cos(a_alpha);
        yv_ += a_v * sin(a_alpha);
    }
};