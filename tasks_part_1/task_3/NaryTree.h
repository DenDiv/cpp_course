#include <vector>

struct tree_t
{
    void *data;
    struct tree_t *top;
    struct tree_t **children;
    size_t num_children;
};
using tree_p = tree_t *;

struct NaryTree
{
    tree_p root;
    NaryTree(int data);
    void add_children(tree_p node, const std::vector<int> &vect);
    void print();
    size_t min_path(tree_p node_1, tree_p node_2);
    std::vector<size_t> path2root(tree_p node);
};