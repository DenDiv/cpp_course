#include <algorithm>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>


#include <Config.h>
#include <RBTree.h>

using NodePtr = Node*;

TEST(FuncTests, LeftRotate)
{
    NodePtr N1 = new Node{5, nullptr, nullptr, nullptr, false};
    NodePtr N2 = new Node{1, N1, nullptr, nullptr, false};
    N1->left = N2;

    NodePtr N3 = new Node{7, N1, nullptr, nullptr, false};
    N1->right = N3;

    NodePtr N4 = new Node{6, N3, nullptr, nullptr, false};
    N3->left = N4;
    NodePtr N5 = new Node{8, N3, nullptr, nullptr, false};
    N3->right = N5;

    RBTree tree{};
    NodePtr root = N1;
    tree.check_leftRotate(root);

    ASSERT_TRUE(tree.get_root() == N3);
    ASSERT_TRUE(N3->parent == nullptr);
    ASSERT_TRUE(N3->left == N1);
    ASSERT_TRUE(N3->right == N5);

    ASSERT_TRUE(N5->parent == N3);
    ASSERT_TRUE(N5->left == nullptr);
    ASSERT_TRUE(N5->right == nullptr);

    ASSERT_TRUE(N1->parent == N3);
    ASSERT_TRUE(N1->left == N2);
    ASSERT_TRUE(N1->right == N4);

    ASSERT_TRUE(N2->parent == N1);
    ASSERT_TRUE(N2->left == nullptr);
    ASSERT_TRUE(N2->right == nullptr);

    ASSERT_TRUE(N4->parent == N1);
    ASSERT_TRUE(N4->left == nullptr);
    ASSERT_TRUE(N4->right == nullptr);
}

TEST(FuncTests, RightRotate)
{
    NodePtr N1 = new Node{7, nullptr, nullptr, nullptr, false};
    NodePtr N2 = new Node{5, N1, nullptr, nullptr, false};
    N1->left = N2;

    NodePtr N3 = new Node{8, N1, nullptr, nullptr, false};
    N1->right = N3;

    NodePtr N4 = new Node{1, N2, nullptr, nullptr, false};
    N2->left = N4;
    NodePtr N5 = new Node{6, N2, nullptr, nullptr, false};
    N2->right = N5;

    RBTree tree{};
    NodePtr root = N1;
    tree.check_rightRotate(root);

    ASSERT_TRUE(tree.get_root() == N2);
    ASSERT_TRUE(N2->parent == nullptr);
    ASSERT_TRUE(N2->left == N4);
    ASSERT_TRUE(N2->right == N1);

    ASSERT_TRUE(N4->parent == N2);
    ASSERT_TRUE(N4->left == nullptr);
    ASSERT_TRUE(N4->right == nullptr);

    ASSERT_TRUE(N1->parent == N2);
    ASSERT_TRUE(N1->left == N5);
    ASSERT_TRUE(N1->right == N3);

    ASSERT_TRUE(N5->parent == N1);
    ASSERT_TRUE(N5->left == nullptr);
    ASSERT_TRUE(N5->right == nullptr);

    ASSERT_TRUE(N3->parent == N1);
    ASSERT_TRUE(N3->left == nullptr);
    ASSERT_TRUE(N3->right == nullptr);
}


TEST(FuncTests, RBCond_1_simple)
{
    // root and leafs are black
    std::vector<int> keys{1, 5, 2, 4, -2, 24, -3, -2, 0, 5, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
        tree.insert(i);

    ASSERT_TRUE(!tree.get_root()->is_red);
}

bool check_color_rec(NodePtr node)
{
    if (node)
    {
        if (node->is_red && node->parent->is_red)
        {
            return false;
        }
        return check_color_rec(node->left) && check_color_rec(node->right);
    }
    else
    {
        return true;
    }
}

TEST(FuncTests, RBCond_2_simple)
{
    // every red node must have black parent node
    std::vector<int> keys{1, 5, 2, 4, -2, 24, -3, -2, 0, 5, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
    {
        tree.insert(i);
        ASSERT_TRUE(check_color_rec(tree.get_root()));
    }
}

void check_black_len(NodePtr node, int cur_len, std::vector<int>& lens)
{
    if (!node)
    {
        cur_len++;
        lens.push_back(cur_len);
        return;
    }

    if (!node->is_red)
        cur_len++;

    check_black_len(node->left, cur_len, lens);
    check_black_len(node->right, cur_len, lens);
}

TEST(FuncTests, RBCond_3_simple)
{
    // black len to leafs must be equal for all nodes
    std::vector<int> keys{1, 5, 2, 4, -2, 24, -3, -2, 0, 5, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
        tree.insert(i);

    std::vector<int> lens;
    check_black_len(tree.get_root(), 0, lens);
    int val = lens[0];
    for (auto i : lens)
        ASSERT_TRUE(val == i);
}

TEST(FuncTests, RBCond_1_total)
{
    // root and leafs are black for test data
    std::fstream datafile;
    std::string line;

    std::string data_file_path = std::string(TEST_DATA_PATH) + "/test_data.dat";
    datafile.open(data_file_path, std::ios::in);

    if (!datafile.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(datafile, line))
    {
        RBTree tree{};
        std::stringstream ss(line);
        char operation;
        int value;

        while (!ss.eof())
        {
            ss >> operation >> value;
            if (operation == 'k')
                tree.insert(value);
            else if (operation == 'm' || operation == 'n')
                continue;
            else
                throw std::runtime_error("unknown operation: " + operation);
        }
        ASSERT_TRUE(!tree.get_root()->is_red);
    }
}

TEST(FuncTests, RBCond_2_total)
{
    // every red node must have black parent node for test data
    std::fstream datafile;
    std::string line;

    std::string data_file_path = std::string(TEST_DATA_PATH) + "/test_data.dat";
    datafile.open(data_file_path, std::ios::in);

    if (!datafile.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(datafile, line))
    {
        RBTree tree{};
        std::stringstream ss(line);
        char operation;
        int value;

        while (!ss.eof())
        {
            ss >> operation >> value;
            if (operation == 'k')
                tree.insert(value);
            else if (operation == 'm' || operation == 'n')
                continue;
            else
                throw std::runtime_error("unknown operation: " + operation);
        }
        ASSERT_TRUE(check_color_rec(tree.get_root()));
    }
}

TEST(FuncTests, RBCond_3_total)
{
    // every red node must have black parent node for test data
    std::fstream datafile;
    std::string line;

    std::string data_file_path = std::string(TEST_DATA_PATH) + "/test_data.dat";
    datafile.open(data_file_path, std::ios::in);

    if (!datafile.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(datafile, line))
    {
        RBTree tree{};
        std::stringstream ss(line);
        char operation;
        int value;

        while (!ss.eof())
        {
            ss >> operation >> value;
            if (operation == 'k')
                tree.insert(value);
            else if (operation == 'm' || operation == 'n')
                continue;
            else
                throw std::runtime_error("unknown operation: " + operation);
        }

        std::vector<int> lens;
        check_black_len(tree.get_root(), 0, lens);
        int val = lens[0];
        for (auto i : lens)
            ASSERT_TRUE(val == i);
    }
}

void check_stats_rer(NodePtr node, std::unordered_map<int, int>& res)
{
    if (node)
    {
        res[node->min_stat] = node->data;
        check_stats_rer(node->left, res);
        check_stats_rer(node->right, res);
    }
}

TEST(FuncTests, check_min_stat)
{
    // root and leafs are black
    std::vector<int> keys{1, 5, 2, 4, -2, -3, 0, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
        tree.insert(i);

    std::sort(keys.begin(), keys.end());
    std::unordered_map<int, int> real_map, tree_map;

    for (int idx = 0; idx < keys.size(); ++idx)
        real_map[idx + 1] = keys[idx];

    check_stats_rer(tree.get_root(), tree_map);
    ASSERT_TRUE(tree_map == real_map);
}

bool compare_trees(NodePtr node1, NodePtr node2)
{
    if (!node1 && !node2)
        return true;

    if (!node1 || !node2)
        return false;

    if (node1->data == node2->data && node1->is_red == node2->is_red && node1->min_stat == node2->min_stat)
        return compare_trees(node1->left, node2->left) && compare_trees(node1->right, node2->right);
    else
        return false;
    
}

TEST(FuncTests, copy_constr)
{
    std::vector<int> keys{1, 5, 2, 4, -2, 24, -3, -2, 0, 5, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
        tree.insert(i);

    RBTree copy_tree{tree};
    ASSERT_TRUE(compare_trees(tree.get_root(), copy_tree.get_root()));
}

TEST(FuncTests, ass_operator)
{
    std::vector<int> keys{1, 5, 2, 4, -2, 24, -3, -2, 0, 5, 8, 3, 15, 12, 13, 125, -12, 24};
    RBTree tree{};
    for (auto i : keys)
        tree.insert(i);

    RBTree new_tree = tree;
    ASSERT_TRUE(compare_trees(tree.get_root(), new_tree.get_root()));
}

TEST(End2EndTests, CheckAns)
{
    // check "minimal stats" and "less than" tasks
    std::fstream datafile, ansfile;
    std::string line;

    std::string data_file_path = std::string(TEST_DATA_PATH) + "/test_data.dat";
    std::string ans_file_path = std::string(TEST_DATA_PATH) + "/test_data.ans";

    datafile.open(data_file_path, std::ios::in);
    ansfile.open(ans_file_path, std::ios::in);

    if (!datafile.is_open() || !ansfile.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    std::vector<std::vector<int>> total_res;
    while (std::getline(datafile, line))
    {
        std::vector<int> curr_res;
        RBTree tree{};
        std::stringstream ss(line);
        char operation;
        int value;

        while (!ss.eof())
        {
            ss >> operation >> value;
            if (operation == 'k')
                tree.insert(value);
            else if (operation == 'm')
                curr_res.push_back(tree.get_min_stat(value));
            else if (operation == 'n')
                curr_res.push_back(tree.get_less_than(value));
            else
                throw std::runtime_error("unknown operation: " + operation);
        }
        total_res.push_back(curr_res);
    }

    std::vector<std::vector<int>> total_ans;
    while (std::getline(ansfile, line))
    {
        std::vector<int> curr_ans;
        std::stringstream ss(line);
        int value;

        while (!ss.eof())
        {
            ss >> value;
            curr_ans.push_back(value);
        }
        total_ans.push_back(curr_ans);
    }

    ASSERT_TRUE(total_ans==total_res);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}