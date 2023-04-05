#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>


struct Node
{
    int data;
    Node* parent;
    Node* left;
    Node* right;
    bool isRed;
};

using NodePtr = Node*;

// Red-Black tree
struct RBTree
{
private:
    void leftRotate(NodePtr node)
    {
        NodePtr p_node = node->parent;
        NodePtr ch_node_r = node->right;
        assert(ch_node_r);

        NodePtr ch_node_r_l = ch_node_r->left;

        if (p_node)
        {
            if (node->data < p_node->data)
                p_node->left = ch_node_r;
            else
                p_node->right = ch_node_r;
        }
        else
        {
            root = ch_node_r;
        }

        ch_node_r->left = node;
        ch_node_r->parent = p_node;

        node->right = ch_node_r_l;
        node->parent = ch_node_r;

        if (ch_node_r_l)
            ch_node_r_l->parent = node;
    }

    void rightRotate(NodePtr node)
    {
        NodePtr p_node = node->parent;
        NodePtr ch_node_l = node->left;
        assert(ch_node_l);

        NodePtr ch_node_l_r = ch_node_l->right;

        if (p_node)
        {
            if (node->data < p_node->data)
                p_node->left = ch_node_l;
            else
                p_node->right = ch_node_l;
        }
        else
        {
            root = ch_node_l;
        }


        ch_node_l->right = node;
        ch_node_l->parent = p_node;

        node->left = ch_node_l_r;
        node->parent = ch_node_l;

        if (ch_node_l_r)
            ch_node_l_r->parent = node;
    }

    void fix_insertion(NodePtr new_node)
    {
        NodePtr p = nullptr;
        NodePtr grandp = nullptr;
        NodePtr uncle = nullptr;
        while (new_node->parent->isRed)
        {
            p = new_node->parent;
            grandp = p->parent;
            // if "father" is left child
            if (p->data < grandp->data)
            {
                uncle = grandp->right;

                // if "uncle" exists and red
                if ((uncle) && (uncle->isRed))
                {
                    p->isRed = false;
                    uncle->isRed = false;
                    grandp->isRed = true;
                    new_node = grandp;
                }
                else
                {
                    // if new_node is right child
                    if (new_node->data > p->data)
                    {
                        new_node = p;
                        leftRotate(new_node);
                        p = new_node->parent;
                        grandp = p->parent;
                    }
                    p->isRed = false;
                    grandp->isRed = true;
                    rightRotate(grandp);
                }
            }
            else
            {
                uncle = grandp->left;
                // if "uncle" exists and red
                if ((uncle) && (uncle->isRed))
                {
                    p->isRed = false;
                    uncle->isRed = false;
                    grandp->isRed = true;
                    new_node = grandp;
                }
                else
                {
                    // if new_node is left child
                    if (new_node->data < p->data)
                    {
                        new_node = p;
                        rightRotate(new_node);
                        p = new_node->parent;
                        grandp = p->parent;
                    }
                    p->isRed = false;
                    grandp->isRed = true;
                    leftRotate(grandp);
                }
            }
            if (new_node == root)
                break;
        }
        root->isRed = false;
    }

    int get_height_rec(NodePtr node, int height)
    {
        if (node)
            return 1 + std::max(get_height_rec(node->left, height), get_height_rec(node->right, height));
        else
            return height;
    }

    void print_rec(NodePtr node)
    {
        if (node)
        {
            print_rec(node->left);
            std::cout << node->data << " ";
            print_rec(node->right);
        }
    }

    NodePtr root;
    size_t val_count;

public:
    RBTree() : root(nullptr), val_count(0){};

    void insert(int key)
    {
        NodePtr new_node = new Node{key, nullptr, nullptr, nullptr, true};
        val_count++;

        if (!root)
        {
            new_node->isRed = false;
            root = new_node;
            return;
        }

        NodePtr p = root;
        NodePtr q = nullptr;

        while (p)
        {
            q = p;
            if (p->data < key)
                p = p->right;
            else if (p->data > key)
                p = p->left;
            else
                {
                    val_count--;
                    return;
                }
        }

        new_node->parent = q;
        if (q->data < key)
            q->right = new_node;
        else
            q->left = new_node;

        fix_insertion(new_node);
    }

    int get_height() { return get_height_rec(root, 0); }

    void print() { print_rec(root); }
};
