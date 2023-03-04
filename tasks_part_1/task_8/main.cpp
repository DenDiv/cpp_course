#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <ctype.h>
#include <stdexcept>
#include <string_view>

namespace main_const
{
    // supported operators
    const char ops[] = {'-',
                        '+',
                        '*',
                        '/',
                        '\0'};

}

bool isnumber(std::string_view str)
{
    for (int i = 0; i < str.length(); ++i)
        if (isdigit(str[i]) == false)
        {
            return false;
        }
    return true;
}

bool isoperator(std::string_view str)
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

    default:
        throw std::runtime_error("unknown op");
    }
    return res;
}

std::vector<std::string_view> split_line(std::string_view expr)
{
    size_t pos = 0;
    std::string_view token;
    constexpr std::string_view delimiter = " ";
    std::vector<std::string_view> values;

    while ((pos = expr.find(delimiter)) != std::string_view::npos)
    {
        token = expr.substr(0, pos);
        values.push_back(token);
        expr.remove_prefix(pos + delimiter.length());
    }
    values.push_back(expr);
    return values;
}

float calc_expr(const std::string_view expr)
{
    std::stack<float> val_stack;
    char op;
    float rvalue, lvalue, op_res;

    std::vector<std::string_view> elements = split_line(expr);

    for (std::string_view e : elements)
    {
        if (isnumber(e))
        {
            val_stack.push(std::stof(static_cast<std::string>(e)));
        }
        else if (isoperator(e))
        {
            op = (e)[0];
            rvalue = static_cast<float>(val_stack.top());
            val_stack.pop();
            lvalue = static_cast<float>(val_stack.top());
            val_stack.pop();
            op_res = make_operation(lvalue, rvalue, op);
            val_stack.push(op_res);
        }
        else
        {
            std::string exc_msg = "Not supported operator: " + static_cast<std::string>(e);
            throw std::invalid_argument(exc_msg);
        }
    }
    return val_stack.top();
}

int main()
{
    /* computes polish expression */
    std::string expr;
    float expr_res;

    std::cout << "print expression: ";
    std::getline(std::cin, expr);
    expr_res = calc_expr(static_cast<const std::string_view>(expr));
    std::cout << "Expr res: " << expr_res;
    return 0;
}
