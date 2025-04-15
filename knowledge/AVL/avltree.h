#ifndef AVLTREE_H
#define AVLTREE_H

// AVL树节点类
class Node
{
public:
    int data;
    Node *left;
    Node *right;
    int height; // AVL树需要记录节点高度

    explicit Node(int data);
};

// AVL树类
class AVLTree
{
private:
    Node *root;

    // 工具方法
    int height(Node *N);
    int getBalance(Node *N);
    Node *rightRotate(Node *y);
    Node *leftRotate(Node *x);
    Node *insert(Node *node, int data);
    Node *minValueNode(Node *node);
    Node *deleteNode(Node *root, int data);
    void inOrderTraversal(Node *root);
    void destroyTree(Node *node);
    Node *findNode(Node *root, int data);

public:
    AVLTree();
    ~AVLTree();

    void insert(int data);
    void remove(int data);
    bool contains(int data);
    void inOrder();
    int getHeight();
};

#endif // AVLTREE_H