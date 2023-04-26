#include <type_traits>

namespace sfinae_ns {
class foo final
{
public:
    foo(int val) : _val(val){};
    int operator+(foo& rhs) { return _val + rhs._val; }

private:
    int _val;
};

class boo final
{};

}  // namespace sfinae_ns


template <typename, typename = void>
struct has_add_operator : std::false_type
{};

template <typename T>
struct has_add_operator<T, std::void_t<decltype(std::declval<T&>() + std::declval<T&>())>> : std::true_type
{};
