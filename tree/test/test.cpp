#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <Config.h>
#define private public
#include <RBTree.h>


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
    tree.root = N1;
    tree.leftRotate(N1);

    ASSERT_TRUE(tree.root == N3);
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
    tree.root = N1;
    tree.rightRotate(N1);

    ASSERT_TRUE(tree.root == N2);
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

    ASSERT_TRUE(!tree.root->isRed);
}

bool check_color_rec(NodePtr node)
{
    if (node)
    {
        if (node->isRed && node->parent->isRed)
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
        ASSERT_TRUE(check_color_rec(tree.root));
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

    if (!node->isRed)
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
    check_black_len(tree.root, 0, lens);
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
        ASSERT_TRUE(!tree.root->isRed);
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
        ASSERT_TRUE(check_color_rec(tree.root));
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
        check_black_len(tree.root, 0, lens);
        int val = lens[0];
        for (auto i : lens)
            ASSERT_TRUE(val == i);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}