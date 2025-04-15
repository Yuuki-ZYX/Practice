#include "rbtree.h"
#include <iostream>

// The color of new node is always red
Node::Node(int data) : data(data), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

RedBlackTree::RedBlackTree() : root(nullptr) {}

RedBlackTree::~RedBlackTree()
{
    // 释放所有内存
    destroyTree(root);
}

void RedBlackTree::destroyTree(Node *node)
{
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

void RedBlackTree::rotateLeft(Node *x)
{
    // Left rotation logic
    Node *y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(Node *x)
{
    // Right rotation logic
    Node *y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void RedBlackTree::fixViolation(Node *pt)
{
    Node *parent_pt = nullptr;
    Node *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color == RED) && (pt->parent != nullptr && pt->parent->color == RED))
    {
        parent_pt = pt->parent;
        grand_parent_pt = parent_pt->parent;

        // 如果没有祖父节点，退出循环
        if (grand_parent_pt == nullptr)
            break;

        // Case A: Parent is left child of grandparent
        if (parent_pt == grand_parent_pt->left)
        {
            Node *uncle_pt = grand_parent_pt->right;

            // Case 1: Uncle is RED - Recoloring needed
            if (uncle_pt != nullptr && uncle_pt->color == RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt; // Move pt up for next iteration
            }
            else // Case 2: Uncle is BLACK (or NULL)
            {
                // LL-Case: pt is left child of parent
                if (pt == parent_pt->left)
                {
                    // Swap colors of parent and grandparent
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    // Right rotate grandparent
                    rotateRight(grand_parent_pt);
                    pt = parent_pt; // Update pt for next iteration
                }
                // LR-Case: pt is right child of parent
                else
                {
                    // First convert to LL-case with left rotation
                    Node *old_parent = parent_pt;
                    rotateLeft(parent_pt);
                    // 现在pt成为旋转后的根节点
                    pt = old_parent;
                    parent_pt = pt->parent;

                    // 执行LL-case处理
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    rotateRight(grand_parent_pt);
                    pt = parent_pt;
                }
            }
        }
        // Case B: Parent is right child of grandparent
        else
        {
            Node *uncle_pt = grand_parent_pt->left;

            // Case 1: Uncle is RED - Recoloring needed
            if ((uncle_pt != nullptr) && (uncle_pt->color == RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt; // Move pt up for next iteration
            }
            // Case 2: Uncle is BLACK (or NULL)
            else
            {
                // RR-Case: pt is right child of parent
                if (pt == parent_pt->right)
                {
                    // Swap colors of parent and grandparent
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    // Left rotate grandparent
                    rotateLeft(grand_parent_pt);
                    pt = parent_pt; // Update pt for next iteration
                }
                // RL-Case: pt is left child of parent
                else
                {
                    // First convert to RR-case with right rotation
                    Node *old_parent = parent_pt;
                    rotateRight(parent_pt);
                    // 现在pt成为旋转后的根节点
                    pt = old_parent;
                    parent_pt = pt->parent;

                    // 执行RR-case处理
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    rotateLeft(grand_parent_pt);
                    pt = parent_pt;
                }
            }
        }
    }

    // Ensure root is always black
    if (root != nullptr)
        root->color = BLACK;
}

Node *RedBlackTree::BSTInsert(Node *root, Node *pt)
{
    // 处理空树情况
    if (root == nullptr)
        return pt;

    // 正常BST插入
    if (pt->data < root->data)
    {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    else
    {
        // 处理重复键 - 这里简单地返回而不插入
        // 也可以根据需要更新值或用其他方式处理
        delete pt; // 防止内存泄漏
        return root;
    }

    return root;
}

void RedBlackTree::inorderUtil(Node *root)
{
    if (root == nullptr)
        return;
    inorderUtil(root->left);
    auto color = root->color ? "Black" : "Red";
    std::cout << root->data << " : " << color << "   ";
    inorderUtil(root->right);
}

void RedBlackTree::insert(const int data)
{
    Node *new_node = new Node(data);
    root = BSTInsert(root, new_node);
    // 只有当节点真的被插入时才修复违规
    if (new_node->parent != nullptr || new_node == root)
        fixViolation(new_node);
}

void RedBlackTree::inorder()
{
    inorderUtil(root);
    std::cout << std::endl;
}

bool RedBlackTree::contains(int data)
{
    return findNode(data) != nullptr;
}

// 查找包含指定值的节点
Node *RedBlackTree::findNode(int data)
{
    Node *current = root;
    while (current != nullptr)
    {
        if (data == current->data)
            return current;
        else if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// 找到子树中的最小节点
Node *RedBlackTree::findMinimum(Node *subtree)
{
    Node *current = subtree;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// 用一棵子树替换另一棵子树
void RedBlackTree::transplant(Node *u, Node *v)
{
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != nullptr)
        v->parent = u->parent;
}

// 修复删除节点后的红黑树性质
void RedBlackTree::fixDeleteViolation(Node *x, Node *xParent, bool isLeft)
{
    while (x != root && (x == nullptr || x->color == BLACK))
    {
        if (x == nullptr)
        {
            // 空节点视为黑色
            if (isLeft)
            {
                // 处理x是左子节点的情况
                Node *w = xParent->right;

                // Case 1: 兄弟节点是红色
                if (w->color == RED)
                {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateLeft(xParent);
                    w = xParent->right;
                }

                // Case 2: 兄弟节点的两个子节点都是黑色
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = xParent;
                    if (x->parent != nullptr)
                    {
                        isLeft = (x == x->parent->left);
                        xParent = x->parent;
                    }
                    else
                    {
                        break; // x变成了根节点
                    }
                }
                else
                {
                    // Case 3: 兄弟节点的右子节点是黑色
                    if (w->right == nullptr || w->right->color == BLACK)
                    {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = xParent->right;
                    }

                    // Case 4: 兄弟节点的右子节点是红色
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    rotateLeft(xParent);
                    x = root; // 终止循环
                    break;
                }
            }
            else
            {
                // 处理x是右子节点的情况 (镜像操作)
                Node *w = xParent->left;

                // Case 1: 兄弟节点是红色
                if (w->color == RED)
                {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateRight(xParent);
                    w = xParent->left;
                }

                // Case 2: 兄弟节点的两个子节点都是黑色
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK))
                {
                    w->color = RED;
                    x = xParent;
                    if (x->parent != nullptr)
                    {
                        isLeft = (x == x->parent->left);
                        xParent = x->parent;
                    }
                    else
                    {
                        break; // x变成了根节点
                    }
                }
                else
                {
                    // Case 3: 兄弟节点的左子节点是黑色
                    if (w->left == nullptr || w->left->color == BLACK)
                    {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = xParent->left;
                    }

                    // Case 4: 兄弟节点的左子节点是红色
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    rotateRight(xParent);
                    x = root; // 终止循环
                    break;
                }
            }
        }
        else
        {
            // x不为空的情况，类似处理
            if (x == xParent->left)
            {
                Node *w = xParent->right;
                // 类似上述处理...
                if (w->color == RED)
                {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateLeft(xParent);
                    w = xParent->right;
                }

                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = xParent;
                    if (x->parent != nullptr)
                    {
                        isLeft = (x == x->parent->left);
                        xParent = x->parent;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if (w->right == nullptr || w->right->color == BLACK)
                    {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = xParent->right;
                    }

                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    rotateLeft(xParent);
                    x = root;
                    break;
                }
            }
            else
            {
                Node *w = xParent->left;
                // 镜像操作...
                if (w->color == RED)
                {
                    w->color = BLACK;
                    xParent->color = RED;
                    rotateRight(xParent);
                    w = xParent->left;
                }

                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK))
                {
                    w->color = RED;
                    x = xParent;
                    if (x->parent != nullptr)
                    {
                        isLeft = (x == x->parent->left);
                        xParent = x->parent;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if (w->left == nullptr || w->left->color == BLACK)
                    {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = xParent->left;
                    }

                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    rotateRight(xParent);
                    x = root;
                    break;
                }
            }
        }
    }

    if (x != nullptr)
        x->color = BLACK;
}

// 删除节点的主函数
void RedBlackTree::remove(int data)
{
    Node *z = findNode(data);
    if (z == nullptr)
        return; // 节点不存在

    Node *y = z;
    Node *x = nullptr;
    Node *xParent = nullptr;
    bool isLeft = false;
    Color originalColor = y->color;

    if (z->left == nullptr)
    {
        // 情况1：左子节点为空
        x = z->right;
        xParent = z->parent;
        isLeft = (z->parent != nullptr && z == z->parent->left);
        transplant(z, z->right);
    }
    else if (z->right == nullptr)
    {
        // 情况2：右子节点为空
        x = z->left;
        xParent = z->parent;
        isLeft = (z->parent != nullptr && z == z->parent->left);
        transplant(z, z->left);
    }
    else
    {
        // 情况3：有两个子节点
        y = findMinimum(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z)
        {
            // y是z的直接右子节点
            if (x != nullptr)
                x->parent = y;
            xParent = y;
            isLeft = false;
        }
        else
        {
            // y在z的右子树中但不是直接子节点
            xParent = y->parent;
            isLeft = (y == y->parent->left);
            transplant(y, y->right);
            y->right = z->right;
            if (y->right != nullptr)
                y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    // 如果删除的是黑色节点，需要修复红黑树性质
    if (originalColor == BLACK)
    {
        if (x != nullptr || xParent != nullptr)
            fixDeleteViolation(x, xParent, isLeft);
    }

    delete z; // 释放内存
}