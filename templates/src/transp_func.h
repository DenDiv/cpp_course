
template <typename Fun, typename... Args>
decltype(auto) transparent(Fun fun, Args&&... args)
{
    return std::forward<Fun>(fun) fun(std::forward<Args>(args)...);
}