#ifndef RBTREE_H
#define RBTREE_H

// 节点颜色枚举
enum Color
{
    RED,
    BLACK
};

// 节点类定义
class Node
{
public:
    int data;
    Color color;
    Node *parent, *left, *right;

    // 构造函数
    explicit Node(int data);
};

// 红黑树类定义
class RedBlackTree
{
private:
    Node *root;

    // 私有辅助方法
    void rotateLeft(Node *x);
    void rotateRight(Node *y);
    void fixViolation(Node *pt);
    Node *BSTInsert(Node *root, Node *pt);
    void inorderUtil(Node *root);
    void destroyTree(Node *node);

    // 删除操作的辅助方法
    Node *findNode(int data);
    Node *findMinimum(Node *subtree);
    void transplant(Node *u, Node *v);
    void fixDeleteViolation(Node *x, Node *xParent, bool isLeft);

public:
    // 构造和析构
    RedBlackTree();
    ~RedBlackTree();

    // 公共接口方法
    void insert(const int data);
    void remove(int data);
    void inorder();
    bool contains(int data);
};

#endif // RBTREE_H