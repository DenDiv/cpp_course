#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <ctype.h>
#include <stdexcept>

float calc_expr(std::string &expr);
std::vector<std::string> split_line(std::string &str);
bool isnumber(std::string &str);
bool isoperator(std::string &str);
float make_operation(float l_value, float r_value, char op);

namespace main_const
{
    // supported operators
    const char ops[] = {'-',
                        '+',
                        '*',
                        '/',
                        '\0'};

}

int main()
{
    /* computes polish expression */
    std::string expr;
    float expr_res;

    getline(std::cin, expr);
    expr_res = calc_expr(expr);
    std::cout << "Expr res: " << expr_res;
    return 0;
}

float calc_expr(std::string &expr)
{
    std::stack<float> val_stack;
    char op;
    float rvalue, lvalue, op_res;

    std::vector<std::string> elements = split_line(expr);
    std::vector<std::string>::iterator it;

    for (it = elements.begin(); it != elements.end(); ++it)
    {
        if (isnumber(*it))
        {
            val_stack.push(std::stof(*it));
        }
        else if (isoperator(*it))
        {
            op = (*it)[0];
            rvalue = static_cast<float>(val_stack.top());
            val_stack.pop();
            lvalue = static_cast<float>(val_stack.top());
            val_stack.pop();
            op_res = make_operation(lvalue, rvalue, op);
            val_stack.push(op_res);
        }
        else
        {
            std::string exc_msg = "Not supported operator: " + *it;
            throw std::invalid_argument(exc_msg);
        }
    }
    return val_stack.top();
}

std::vector<std::string> split_line(std::string &expr)
{
    size_t pos = 0;
    std::string token;
    std::string delimiter = " ";
    std::vector<std::string> values;

    while ((pos = expr.find(delimiter)) != std::string::npos)
    {
        token = expr.substr(0, pos);
        values.push_back(token);
        expr.erase(0, pos + delimiter.length());
    }
    values.push_back(expr);
    return values;
}

bool isnumber(std::string &str)
{
    for (int i = 0; i < str.length(); ++i)
        if (isdigit(str[i]) == false)
        {
            return false;
        }
    return true;
}

bool isoperator(std::string &str)
{
    if (str.length() != 1)
    {
        return false;
    }

    char op = str[0];
    for (size_t i = 0; i < strlen(main_const::ops); ++i)
    {
        if (op == main_const::ops[i])
        {
            return true;
        }
    }
    return false;
}

float make_operation(float l_value, float r_value, char op)
{
    float res;
    switch (op)
    {
    case '-':
        res = l_value - r_value;
        break;
    case '+':
        res = l_value + r_value;
        break;
    case '*':
        res = l_value * r_value;
        break;
    case '/':
        res = l_value / r_value;
    }
    return res;
}


