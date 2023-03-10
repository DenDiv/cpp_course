#include <iostream>
#include "NaryTree.h"

int main()
{
    /* N-ary min path task */
    NaryTree tree(0);
    std::vector<int> vec_1 = {1, 2, 3};
    std::vector<int> vec_2 = {4, 5, 6};
    std::vector<int> vec_3 = {7, 8};
    std::vector<int> vec_4 = {9, 10};

    tree.add_children(tree.root, vec_1);
    tree.add_children(tree.root->children[0], vec_2);
    tree.add_children(tree.root->children[0]->children[1], vec_3);
    tree.add_children(tree.root->children[2], vec_4);

    std::cout << "Source Tree: " << std::endl;
    tree.print();
    tree_p node_1, node_2;
    size_t min_path;
    node_1 = tree.root->children[0]->children[1]->children[1];
    node_2 = tree.root->children[2]->children[1];

    min_path = tree.min_path(node_1, node_2);
    std::cout << "Min path: " << min_path;
    return 0;
}