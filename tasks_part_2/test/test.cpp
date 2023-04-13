#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <thread>


#define private public
#include <Config.h>
#include <ball_game.h>

constexpr float eps = 0.0001f;

void play_ball(Ball& b)
{
    std::cout << "start play thread id: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    b.fly(10);
    std::cout << "end play thread id: " << std::this_thread::get_id() << std::endl;
}

TEST(FuncTests, ball_game_simple)
{
    Ball b1{0.0, 0.0, 1.0, 1.0};
    std::thread t1(play_ball, std::ref(b1));
    std::thread t2(play_ball, std::ref(b1));
    std::thread t3(play_ball, std::ref(b1));
    t1.join();
    t2.join();
    t3.join();
    ASSERT_TRUE(std::abs(b1.x_ - 30.0) < eps);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}