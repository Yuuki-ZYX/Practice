#include "avltree.h"
#include <iostream>
#include <algorithm>

// 节点构造函数
Node::Node(int data) : data(data), left(nullptr), right(nullptr), height(1) {}

// AVL树构造函数
AVLTree::AVLTree() : root(nullptr) {}

// 析构函数
AVLTree::~AVLTree()
{
    destroyTree(root);
}

// 递归释放节点
void AVLTree::destroyTree(Node *node)
{
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// 获取节点高度
int AVLTree::height(Node *N)
{
    if (N == nullptr)
        return 0;
    return N->height;
}

// 获取节点的平衡因子
int AVLTree::getBalance(Node *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

// 右旋操作
Node *AVLTree::rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // 执行旋转
    x->right = y;
    y->left = T2;

    // 更新高度
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    // 返回新的根节点
    return x;
}

// 左旋操作
Node *AVLTree::leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // 执行旋转
    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    // 返回新的根节点
    return y;
}

// 内部插入方法（递归）
Node *AVLTree::insert(Node *node, int data)
{
    // 执行普通BST插入
    if (node == nullptr)
        return new Node(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else // 重复键不允许
        return node;

    // 更新当前节点的高度
    node->height = 1 + std::max(height(node->left), height(node->right));

    // 获取平衡因子
    int balance = getBalance(node);

    // 处理不平衡情况

    // 左左情况
    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    // 右右情况
    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    // 左右情况
    if (balance > 1 && data > node->left->data)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // 右左情况
    if (balance < -1 && data < node->right->data)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // 返回未修改的节点指针
    return node;
}

// 查找树中最小值的节点
Node *AVLTree::minValueNode(Node *node)
{
    Node *current = node;

    // 循环查找最左叶子节点
    while (current->left != nullptr)
        current = current->left;

    return current;
}

// 内部删除方法（递归）
Node *AVLTree::deleteNode(Node *root, int data)
{
    // 标准BST删除
    if (root == nullptr)
        return root;

    // 如果要删除的键小于根节点，则在左子树中递归删除
    if (data < root->data)
        root->left = deleteNode(root->left, data);

    // 如果要删除的键大于根节点，则在右子树中递归删除
    else if (data > root->data)
        root->right = deleteNode(root->right, data);

    // 如果键等于根节点，则这个节点将被删除
    else
    {
        // 节点只有一个子节点或没有子节点
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            Node *temp = root->left ? root->left : root->right;

            // 没有子节点的情况
            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else               // 一个子节点的情况
                *root = *temp; // 复制非空子节点的内容到该节点

            delete temp;
        }
        else
        {
            // 有两个子节点的情况：获取中序后继节点（右子树中的最小节点）
            Node *temp = minValueNode(root->right);

            // 将中序后继节点的数据复制到该节点
            root->data = temp->data;

            // 删除中序后继节点
            root->right = deleteNode(root->right, temp->data);
        }
    }

    // 如果树在删除后只剩下一个节点，直接返回
    if (root == nullptr)
        return root;

    // 更新高度
    root->height = 1 + std::max(height(root->left), height(root->right));

    // 获取平衡因子
    int balance = getBalance(root);

    // 处理不平衡情况

    // 左左情况
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // 左右情况
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // 右右情况
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // 右左情况
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// 中序遍历辅助函数
void AVLTree::inOrderTraversal(Node *root)
{
    if (root != nullptr)
    {
        inOrderTraversal(root->left);
        std::cout << root->data << " [" << root->height << "] ";
        inOrderTraversal(root->right);
    }
}

// 公共接口：插入
void AVLTree::insert(int data)
{
    root = insert(root, data);
}

// 公共接口：删除
void AVLTree::remove(int data)
{
    root = deleteNode(root, data);
}

// 公共接口：中序遍历
void AVLTree::inOrder()
{
    inOrderTraversal(root);
    std::cout << std::endl;
}

// 获取树的高度
int AVLTree::getHeight()
{
    return height(root);
}

// 查找节点
Node *AVLTree::findNode(Node *root, int data)
{
    if (root == nullptr || root->data == data)
        return root;

    if (data < root->data)
        return findNode(root->left, data);

    return findNode(root->right, data);
}

// 公共接口：检查是否包含某值
bool AVLTree::contains(int data)
{
    return findNode(root, data) != nullptr;
}

int main()
{
    AVLTree avl;

    // 插入一些数据
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.insert(40);
    avl.insert(50);
    avl.insert(25);

    std::cout << "inOrder ";
    avl.inOrder();

    std::cout << "height: " << avl.getHeight() << std::endl;

    // 删除一些节点
    avl.remove(20);
    std::cout << "delete 20: ";
    avl.inOrder();

    avl.remove(30);
    std::cout << "delete 30: ";
    avl.inOrder();

    // 检查树中是否存在某值
    std::cout << "include ? " << (avl.contains(40) ? "yes" : "no") << " 40" << std::endl;
    std::cout << "incldue ? " << (avl.contains(20) ? "yes" : "no") << " 20" << std::endl;

    return 0;
}