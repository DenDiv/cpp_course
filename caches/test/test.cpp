#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <cache.h>
#include <Config.h>

struct page_t
{
    page_t(int key) : id(key) {}
    size_t id;
};
// slow get page imitation
page_t slow_get_page(int key) { return page_t{key}; }

TEST(CacheTests, LRU_cache)
{
    std::fstream datafile, ansfile;
    std::string line, ans;
    size_t line_counter = 0;

    std::string data_file_path = std::string(TEST_DATA_PATH) + "/test_data_lru.dat";
    std::string ans_file_path = std::string(TEST_DATA_PATH) + "/test_data_lru.ans";

    datafile.open(data_file_path, std::ios::in);
    ansfile.open(ans_file_path, std::ios::in);

    if (!datafile.is_open() || !ansfile.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(datafile, line))
    {
        std::stringstream ss(line);
        int hits = 0;
        size_t m;
        int n;

        ss >> m >> n;
        caches::lru_cache<page_t, int> c{m};
        for (int i = 0; i < n; ++i)
        {
            int q;
            ss >> q;
            if (c.lookup_update(q, slow_get_page))
                hits++;
        }

        std::getline(ansfile, ans);
        if (std::stoi(ans) != hits)
        {
            std::cout << "ans isn't correct, line: " << line_counter;
            ASSERT_TRUE(0);
        }
        line_counter++;
    }
    ASSERT_TRUE(1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}