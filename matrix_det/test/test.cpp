#include <algorithm>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>


#include <Config.h>
#include <matrix.h>

constexpr float eps = 0.0001;

TEST(FuncTests, MatrixConstr)
{
    int ival = 5;
    float fval = 5.45f;

    Matrix<int> m1{3, 3, ival};
    m1.print();
    std::cout << std::endl;

    Matrix<float> m2{3, 3, fval};
    m2.print();
    std::cout << std::endl;

    float fl_mbuff_1[9] = {1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f, 7.25f, 8.25f, 9.25f};
    Matrix<float> m3 = Matrix<float>::create<float*>(3, 3, fl_mbuff_1, fl_mbuff_1 + 9);
    m3.print();
    std::cout << std::endl;

    // there calls move constr
    std::vector<int> int_mbuff_1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<int> m4 = Matrix<int>::create<std::vector<int>::iterator>(3, 3, int_mbuff_1.begin(), int_mbuff_1.end());
    m4.print();
    std::cout << std::endl;

    // there calls move constr
    std::vector<int> int_mbuff_2{21, 12, 32, 44, 15, 65, 37, 28, 19};
    m4 = Matrix<int>::create<std::vector<int>::iterator>(3, 3, int_mbuff_2.begin(), int_mbuff_2.end());
    m4.print();
    std::cout << std::endl;
    ASSERT_TRUE(1);
}

TEST(FuncTests, equal)
{
    float fl_mbuff_1[9] = {1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f, 7.25f, 8.25f, 9.25f};
    float fl_mbuff_2[9] = {1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f, 7.25f, 8.25f, 9.25f};
    Matrix<float> m1 = Matrix<float>::create<float*>(3, 3, fl_mbuff_1, fl_mbuff_1 + 9);
    Matrix<float> m2 = Matrix<float>::create<float*>(3, 3, fl_mbuff_2, fl_mbuff_2 + 9);
    ASSERT_TRUE(m1.equal(m2));
}

TEST(FuncTests, transpose)
{
    float fl_mbuff_1[9] = {1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f, 7.25f, 8.25f, 9.25f};
    Matrix<float> m1 = Matrix<float>::create<float*>(3, 3, fl_mbuff_1, fl_mbuff_1 + 9);
    Matrix<float> m2 = m1;
    m1.transpose();
    ASSERT_TRUE(!m1.equal(m2));
    m1.transpose();
    ASSERT_TRUE(m1.equal(m2));
}

TEST(FuncTests, determinant)
{
    int int_mbuff_1[9] = {1, 2, 5, 2, 3, 1, 1, 4, 2};
    Matrix<int> m1 = Matrix<int>::create<int*>(3, 3, int_mbuff_1, int_mbuff_1 + 9);
    int det_m1 = m1.det();
    ASSERT_TRUE(det_m1 == 21);

    float fl_mbuff_1[9] = {1.50f, 2.00f, 5.00f, 2.00f, 3.50f, 1.00f, 1.00f, 4.00f, 2.50f};
    Matrix<float> m2 = Matrix<float>::create<float*>(3, 3, fl_mbuff_1, fl_mbuff_1 + 9);
    float det_m2 = m2.det();
    ASSERT_TRUE(std::abs(det_m2 - 21.625) < eps);
}

TEST(End2EndTests, det_test)
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

    std::vector<std::vector<int>> int_matr;
    std::vector<int> int_dim;
    std::vector<std::vector<float>> float_matr;
    std::vector<int> float_dim;

    while (std::getline(datafile, line))
    {
        std::stringstream ss(line);
        char data_type;
        int dim;

        ss >> data_type >> dim;
        if (data_type == 'i')
        {
            int val;
            std::vector<int> curr_int_matr;
            while (!ss.eof())
            {
                ss >> val;
                curr_int_matr.push_back(val);
            }
            int_matr.push_back(curr_int_matr);
            int_dim.push_back(dim);
        }
        else
        {
            float val;
            std::vector<float> curr_float_matr;
            while (!ss.eof())
            {
                ss >> val;
                curr_float_matr.push_back(val);
            }
            float_matr.push_back(curr_float_matr);
            float_dim.push_back(dim);
        }
    }

    std::vector<int> int_ans;
    std::vector<float> float_ans;
    char data_type;
    while (std::getline(ansfile, line))
    {
        std::stringstream ss(line);
        ss >> data_type;
        if (data_type == 'i')
        {
            int val;
            ss >> val;
            int_ans.push_back(val);
        }
        else
        {
            float val;
            ss >> val;
            float_ans.push_back(val);
        }
    }

    std::vector<int> int_pred;
    std::vector<float> float_pred;

    for (int i = 0; i < int_matr.size(); ++i)
    {
        Matrix<int> matr = Matrix<int>::create<std::vector<int>::iterator>(int_dim[i], int_dim[i], int_matr[i].begin(), int_matr[i].end());
        int_pred.push_back(matr.det());
    }

    for (int i = 0; i < float_matr.size(); ++i)
    {
        Matrix<float> matr =
            Matrix<float>::create<std::vector<float>::iterator>(float_dim[i], float_dim[i], float_matr[i].begin(), float_matr[i].end());
        float_pred.push_back(matr.det());
    }

    for (int i = 0; i < int_pred.size(); ++i)
    {
        ASSERT_TRUE(std::abs(int_pred[i] - int_ans[i]) <= 1);
    }

    for (int i = 0; i < float_pred.size(); ++i)
    {
        ASSERT_TRUE(std::abs(float_pred[i] - float_ans[i]) < eps);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}