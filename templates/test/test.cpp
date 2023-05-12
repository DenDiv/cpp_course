#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <sfinae.h>
#include <transp_func.h>


TEST(FuncTests, sfinae_task_1)
{
    /*
    Task 1: Напишите SFINAE определитель, отвечающий на вопрос,
            есть ли в классе перегруженный оператор сложения
    */

    ASSERT_TRUE(has_add_operator<sfinae_ns::foo>{});
    ASSERT_FALSE(has_add_operator<sfinae_ns::boo>{});
}

TEST(FuncTests, sfinae_task_2)
{
    /*
    Task 2: Напишите SFINAE определитель, добавляющий (если это возможно)
            правую ссылку к типу или оставляющий тип неизменным. Скажем
            int --> int&&, но int[10] --> int[10]
    */

    // solution - it is std::add_rvalue_reference

    int i = 0;
    int& l = i;
    int arr[] = {1, 2, 3};
    using t1 = typename std::add_rvalue_reference<decltype(i)>::type;
    using t2 = typename std::add_rvalue_reference<decltype(l)>::type;
    using t3 = typename std::add_rvalue_reference<decltype(arr[0])>::type;
    using t4 = typename std::add_rvalue_reference<decltype(std::move(i))>::type;

    ASSERT_TRUE(std::is_rvalue_reference<t1>{});
    ASSERT_TRUE(std::is_lvalue_reference<t2>{});
    ASSERT_TRUE(std::is_lvalue_reference<t3>{});
    ASSERT_TRUE(std::is_rvalue_reference<t4>{});
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}