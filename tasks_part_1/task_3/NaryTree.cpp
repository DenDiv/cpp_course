#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "NaryTree.h"

NaryTree::NaryTree(int value)
{
    root = new tree_t();
    root->data = new int;
    *((int *)root->data) = value;
    root->top = NULL;
    root->children = NULL;
    root->num_children = 0;
}

void NaryTree::add_children(tree_p node, const std::vector<int> &vect)
{
    if (node->children != NULL)
    {
        throw std::invalid_argument("node already has children");
    }

    size_t num_children = vect.size();
    if (num_children == 0)
    {
        throw std::length_error("empty vect");
    }

    node->children = new tree_p[num_children];
    node->num_children = num_children;
    for (size_t i = 0; i < num_children; ++i)
    {
        node->children[i] = new tree_t();
        node->children[i]->data = new int;
        *((int *)node->children[i]->data) = vect[i];
        node->children[i]->top = node;
        node->children[i]->children = NULL;
        node->children[i]->num_children = 0;
    }
}

std::vector<size_t> NaryTree::path2root(tree_p node)
{
    /* Path from root to received node*/
    std::vector<size_t> path;
    size_t addr;
    while (node != NULL)
    {
        addr = reinterpret_cast<size_t>(node);
        path.push_back(addr);
        node = node->top;
    }
    return path;
}

size_t NaryTree::min_path(tree_p node_1, tree_p node_2)
{
    /* finds min path between 2 nodes*/
    std::vector<size_t> path_1, path_2;
    size_t lcw_dist = 0;
    path_1 = path2root(node_1);
    path_2 = path2root(node_2);
    std::reverse(path_1.begin(), path_1.end());
    std::reverse(path_2.begin(), path_2.end());

    // find dist btw root and Lowest Common Ancestor
    for (size_t i = 0;; ++i)
    {
        if (path_1[i] != path_2[i])
        {
            break;
        }
        lcw_dist++;
    }
    return path_1.size() + path_2.size() - 2 * lcw_dist;
}

void NaryTree::print()
{
    /*For simplicity suppose that all data values are different*/
    std::queue<tree_p> q;
    std::string print_ch = "";
    tree_p visited_node;
    int *pInt;
    q.push(root);

    while (!q.empty())
    {
        visited_node = q.front();
        q.pop();
        pInt = static_cast<int *>(visited_node->data);
        print_ch += std::to_string(*pInt) + ": ";

        for (size_t i = 0; i < visited_node->num_children; i++)
        {
            pInt = static_cast<int *>(visited_node->children[i]->data);
            print_ch += std::to_string(*pInt) + ", ";
            q.push(visited_node->children[i]);
        }
        print_ch += ";\n";
    }
    std::cout << print_ch;
}