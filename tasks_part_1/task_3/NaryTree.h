#include <vector>

typedef struct tree_tag
{
    void *data;
    struct tree_tag *top;
    struct tree_tag **children;
    size_t num_children;
} tree_t, *tree_p;

struct NaryTree
{
    tree_p root;
    NaryTree(int data);
    void add_children(tree_p node, const std::vector<int> &vect);
    void print();
    size_t min_path(tree_p node_1, tree_p node_2);
    std::vector<size_t> path2root(tree_p node);
};