#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <random>
#include <vector>

// Treap节点类
class TreapNode
{
public:
    int key;      // 键值（满足二叉搜索树性质）
    int priority; // 优先级（满足堆性质）
    TreapNode *left;
    TreapNode *right;

    TreapNode(int k);
};

// Treap类
class Treap
{
private:
    TreapNode *root;
    std::mt19937 rng; // 随机数生成器

    // 内部工具方法
    void destroyTree(TreapNode *node);
    TreapNode *rightRotate(TreapNode *y);
    TreapNode *leftRotate(TreapNode *x);
    TreapNode *insert(TreapNode *root, int key);
    TreapNode *remove(TreapNode *root, int key);
    TreapNode *search(TreapNode *root, int key);
    void inorderTraversal(TreapNode *node);
    int getRandomPriority();
    void printStructure(TreapNode *node, std::string prefix, bool isLeft);

public:
    Treap();
    ~Treap();

    void insert(int key);
    void remove(int key);
    bool search(int key);
    void inorder();
    void printTree();
    std::vector<int> getKeysInRange(int start, int end);
    void getKeysInRange(TreapNode *node, int start, int end, std::vector<int> &result);
};

#endif // TREAP_H