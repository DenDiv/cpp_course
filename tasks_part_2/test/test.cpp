#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>


#include <Config.h>
#include <ball_game.h>

constexpr float eps = 0.0001f;
constexpr double PI = 3.1415926535897932384626;

void play_ball(Player& p, std::vector<std::shared_ptr<BallVirt>>& balls)
{
    for (auto& b : balls)
    {
        b->push(1.0, PI / 4);
        b->fly(1.0);
        b->print();
    }
}

TEST(FuncTests, ball_game)
{
    std::vector<std::shared_ptr<BallVirt>> ball_vec;
    ball_vec.push_back(std::make_shared<BallSimple>(1.0, 1.0, 0.0, 0.0, 1.0, 1.0));
    ball_vec.push_back(std::make_shared<BallWind>(1.0, 1.0, 0.1, 0.0, 0.0, 1.0, 1.0));

    Player p1{"dencer"};
    Player p2{"player1"};
    Player p3{"player2"};

    std::thread t1(play_ball, std::ref(p1), std::ref(ball_vec));
    std::thread t2(play_ball, std::ref(p2), std::ref(ball_vec));
    std::thread t3(play_ball, std::ref(p3), std::ref(ball_vec));
    t1.join();
    t2.join();
    t3.join();

    std::vector<double> c1 = ball_vec[0]->get_coords();
    std::vector<double> c2 = ball_vec[1]->get_coords();

    std::cout << c1[0] << " " << c1[1] << std::endl;
    std::cout << c2[0] << " " << c2[1] << std::endl;

    ASSERT_TRUE(1.0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}