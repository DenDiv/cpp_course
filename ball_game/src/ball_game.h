#include <algorithm>
#include <cassert>
#include <iostream>
#include <math.h>
#include <mutex>
#include <string>
#include <string_view>


class BallBase
{
protected:
    double x_;
    double y_;
    double xv_;
    double yv_;
    double t_;

public:
    BallBase(double x, double y, double xv, double yv, double t = 0.0) : x_(x), y_(y), xv_(xv), yv_(yv), t_(t) {}
    virtual ~BallBase() = 0 {}
    virtual void print() = 0 { std::cout << "x: " << x_ << " y: " << y_ << " xv: " << xv_ << " yv: " << yv_ << " t: " << t_ << std::endl; }
    virtual void fly(double t) = 0;
    virtual void push(double a_v, double a_alpha) = 0;

public:
    // common functions
    std::vector<double> get_coords() const { return std::vector<double>{x_, y_}; }
    std::vector<double> get_velocity() const { return std::vector<double>{xv_, yv_}; }
    double get_time() const { return t_; }
};

class BallSimple final : public BallBase
{
private:
    double r_;
    double weight_;
    std::mutex mt_;

public:
    BallSimple(double r, double weight, double x, double y, double xv, double yv, double t = 0.0)
        : BallBase(x, y, xv, yv, t), r_(r), weight_(weight)
    {}

    ~BallSimple() {}

    void fly(double t) override
    {
        assert(t > 0);
        std::lock_guard<std::mutex> lg(mt_);
        x_ += xv_ * t;
        y_ += yv_ * t;
        t_ += t;
    }

    void push(double a_v, double a_alpha) override
    {
        assert(a_v > 0);
        std::lock_guard<std::mutex> lg(mt_);
        xv_ += a_v * cos(a_alpha);
        yv_ += a_v * sin(a_alpha);
    }

    void print() override
    {
        std::lock_guard<std::mutex> lg(mt_);
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "r: " << r_ << " weight: " << weight_ << std::endl;
        BallBase::print();
    }
};

class BallWind final : public BallBase
{
private:
    double r_;
    double weight_;
    double wind_k_;  // same for x and y wind force koef
    std::mutex mt_;

public:
    BallWind(double r, double weight, double wind_speed, double x, double y, double xv, double yv, double t = 0.0)
        : BallBase(x, y, xv, yv, t), r_(r), weight_(weight), wind_k_(wind_speed)
    {}

    ~BallWind() {}

    void fly(double t) override
    {
        assert(t > 0);
        std::lock_guard<std::mutex> lg(mt_);
        x_ += wind_k_ * xv_ * t;
        y_ += wind_k_ * yv_ * t;
        t_ += t;
    }

    void push(double a_v, double a_alpha) override
    {
        assert(a_v > 0);
        std::lock_guard<std::mutex> lg(mt_);
        xv_ += a_v * cos(a_alpha);
        yv_ += a_v * sin(a_alpha);
    }

    void print() override
    {
        std::lock_guard<std::mutex> lg(mt_);
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "r: " << r_ << " weight: " << weight_ << " wind_k: " << wind_k_ << std::endl;
        BallBase::print();
    }
};

class Player final
{
public:
    Player(std::string_view name = "huilack"): name_(name) {}

    void push_ball(BallBase& b, double a_v, double a_alpha) { b.push(a_v, a_alpha); }

    void fly_ball(BallBase& b, double t) { b.fly(t); }

    std::string name_;
};