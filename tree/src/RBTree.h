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
    bool is_red;
    int min_stat;  // keep sorting position
};


// Red-Black tree
struct RBTree
{
    using NodePtr = Node*;

public:
    RBTree() : root_(nullptr), val_count_(0){};

    RBTree(const RBTree& rhs) : val_count_(rhs.val_count_)
    {
        root_ = new Node{rhs.root_->data, nullptr, nullptr, nullptr, rhs.root_->is_red, rhs.root_->min_stat};
        rec_copy(rhs.root_, root_);
    }

    RBTree& operator=(const RBTree& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        val_count_ = rhs.val_count_;
        destroy_tree(root_);
        root_ = new Node{rhs.root_->data, nullptr, nullptr, nullptr, rhs.root_->is_red, rhs.root_->min_stat};
        rec_copy(rhs.root_, root_);
        return *this;
    }

    ~RBTree() { destroy_tree(root_); }

public:
    void insert(int key)
    {
        NodePtr new_node = new Node{key, nullptr, nullptr, nullptr, true, 1};
        val_count_++;

        if (!root_)
        {
            new_node->is_red = false;
            root_ = new_node;
            return;
        }

        NodePtr p = root_;
        NodePtr q = nullptr;

        while (p)
        {
            q = p;
            if (p->data < key)
            {
                p = p->right;
            }
            else if (p->data > key)
            {
                p->min_stat++;
                if (p->right)
                    update_stats(p->right);
                p = p->left;
            }
            else
            {
                val_count_--;
                return;
            }
        }

        new_node->parent = q;
        if (q->data < key)
        {
            new_node->min_stat = q->min_stat + 1;
            q->right = new_node;
        }
        else
        {
            new_node->min_stat = q->min_stat - 1;
            q->left = new_node;
        }

        fix_insertion(new_node);
    }

    int get_height() { return get_height_rec(root_, 0); }

    void print() { print_rec(root_); }

    // returns min stat data
    int get_min_stat(int min_stat)
    {
        if (min_stat > val_count_ || min_stat <= 0)
        {
            throw std::runtime_error("Invalid min_stat: " + min_stat);
        }
        return get_min_stat_rec(root_, min_stat);
    }

    // returns count of less values than input key
    int get_less_than(int key)
    {
        NodePtr p = root_;
        NodePtr q = nullptr;

        while (p)
        {
            q = p;
            if (p->data < key)
            {
                p = p->right;
            }
            else if (p->data > key)
            {
                p = p->left;
            }
            else
            {
                return p->min_stat - 1;
            }
        }

        if (q->data < key)
        {
            return q->min_stat;
        }
        else
        {
            return q->min_stat - 1;
        }
    }

public:
    void set_root(NodePtr root)
    {
        root_ = root;
    }

    NodePtr get_root()
    {
        return root_;
    }

    void check_leftRotate(NodePtr node)
    {
        leftRotate(node);
    }

    void check_rightRotate(NodePtr node)
    {
        rightRotate(node);
    }

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
            {
                p_node->left = ch_node_r;
            }
            else
            {
                p_node->right = ch_node_r;
            }
        }
        else
        {
            root_ = ch_node_r;
        }

        ch_node_r->left = node;
        ch_node_r->parent = p_node;

        node->right = ch_node_r_l;
        node->parent = ch_node_r;

        if (ch_node_r_l)
        {
            ch_node_r_l->parent = node;
        }
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
            {
                p_node->left = ch_node_l;
            }
            else
            {
                p_node->right = ch_node_l;
            }
        }
        else
        {
            root_ = ch_node_l;
        }


        ch_node_l->right = node;
        ch_node_l->parent = p_node;

        node->left = ch_node_l_r;
        node->parent = ch_node_l;

        if (ch_node_l_r)
        {
            ch_node_l_r->parent = node;
        }
    }

    void fix_insertion(NodePtr new_node)
    {
        NodePtr p = nullptr;
        NodePtr grandp = nullptr;
        NodePtr uncle = nullptr;
        while (new_node->parent->is_red)
        {
            p = new_node->parent;
            grandp = p->parent;
            // if "father" is left child
            if (p->data < grandp->data)
            {
                uncle = grandp->right;

                // if "uncle" exists and red
                if ((uncle) && (uncle->is_red))
                {
                    p->is_red = false;
                    uncle->is_red = false;
                    grandp->is_red = true;
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
                    p->is_red = false;
                    grandp->is_red = true;
                    rightRotate(grandp);
                }
            }
            else
            {
                uncle = grandp->left;
                // if "uncle" exists and red
                if ((uncle) && (uncle->is_red))
                {
                    p->is_red = false;
                    uncle->is_red = false;
                    grandp->is_red = true;
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
                    p->is_red = false;
                    grandp->is_red = true;
                    leftRotate(grandp);
                }
            }
            if (new_node == root_)
            {
                break;
            }
        }
        root_->is_red = false;
    }

    int get_height_rec(NodePtr node, int height)
    {
        if (node)
        {
            return 1 + std::max(get_height_rec(node->left, height), get_height_rec(node->right, height));
        }
        else
        {
            return height;
        }
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

    void update_stats(NodePtr node)
    {
        if (node)
        {
            node->min_stat++;
            update_stats(node->left);
            update_stats(node->right);
        }
    }

    int get_min_stat_rec(NodePtr node, int min_stat)
    {
        if (min_stat == node->min_stat)
        {
            return node->data;
        }
        else if (min_stat < node->min_stat)
        {
            return get_min_stat_rec(node->left, min_stat);
        }
        else
        {
            return get_min_stat_rec(node->right, min_stat);
        }
    }

    void destroy_tree(NodePtr node)
    {
        if (node)
        {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
        }
    }

    void rec_copy(NodePtr src, NodePtr dst)
    {
        if (src)
        {
            if (src->left)
            {
                dst->left = new Node{src->left->data, dst, nullptr, nullptr, src->left->is_red, src->left->min_stat};
                rec_copy(src->left, dst->left);
            }
            if (src->right)
            {
                dst->right = new Node{src->right->data, dst, nullptr, nullptr, src->right->is_red, src->right->min_stat};
                rec_copy(src->right, dst->right);
            }
        }
    }

    NodePtr root_;
    size_t val_count_;
};
