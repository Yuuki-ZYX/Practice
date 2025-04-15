#include "treap.h"
#include <string>
#include <chrono>

// 节点构造函数
TreapNode::TreapNode(int k) : key(k), left(nullptr), right(nullptr) {}

// Treap构造函数
Treap::Treap() : root(nullptr)
{
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

// 析构函数
Treap::~Treap()
{
    destroyTree(root);
}

// 递归释放节点
void Treap::destroyTree(TreapNode *node)
{
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// 生成随机优先级
int Treap::getRandomPriority()
{
    return rng();
}

// 右旋操作
TreapNode *Treap::rightRotate(TreapNode *y)
{
    TreapNode *x = y->left;
    TreapNode *T2 = x->right;

    // 执行旋转
    x->right = y;
    y->left = T2;

    return x; // 新的根节点
}

// 左旋操作
TreapNode *Treap::leftRotate(TreapNode *x)
{
    TreapNode *y = x->right;
    TreapNode *T2 = y->left;

    // 执行旋转
    y->left = x;
    x->right = T2;

    return y; // 新的根节点
}

// 内部插入方法（递归）
TreapNode *Treap::insert(TreapNode *root, int key)
{
    // 如果树为空，创建新节点
    if (root == nullptr)
    {
        TreapNode *newNode = new TreapNode(key);
        newNode->priority = getRandomPriority();
        return newNode;
    }

    // 按照BST规则插入
    if (key <= root->key)
    {
        root->left = insert(root->left, key);

        // 如果堆性质被破坏，右旋恢复
        if (root->left != nullptr && root->left->priority > root->priority)
            root = rightRotate(root);
    }
    else
    {
        root->right = insert(root->right, key);

        // 如果堆性质被破坏，左旋恢复
        if (root->right != nullptr && root->right->priority > root->priority)
            root = leftRotate(root);
    }

    return root;
}

// 内部删除方法（递归）
TreapNode *Treap::remove(TreapNode *root, int key)
{
    // 如果树为空
    if (root == nullptr)
        return root;

    // 根据键值搜索节点
    if (key < root->key)
    {
        root->left = remove(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = remove(root->right, key);
    }
    else // 找到要删除的节点
    {
        // 如果是叶节点，直接删除
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            return nullptr;
        }
        // 如果只有右子树
        else if (root->left == nullptr)
        {
            TreapNode *temp = root->right;
            delete root;
            return temp;
        }
        // 如果只有左子树
        else if (root->right == nullptr)
        {
            TreapNode *temp = root->left;
            delete root;
            return temp;
        }
        // 如果有两个子节点，根据优先级决定旋转方向
        else
        {
            // 左子节点优先级大，右旋
            if (root->left->priority > root->right->priority)
            {
                root = rightRotate(root);
                root->right = remove(root->right, key);
            }
            // 右子节点优先级大或相等，左旋
            else
            {
                root = leftRotate(root);
                root->left = remove(root->left, key);
            }
        }
    }
    return root;
}

// 内部搜索方法
TreapNode *Treap::search(TreapNode *root, int key)
{
    // 如果树为空或找到对应的键
    if (root == nullptr || root->key == key)
        return root;

    // 如果键小于当前节点，在左子树中搜索
    if (key < root->key)
        return search(root->left, key);

    // 如果键大于当前节点，在右子树中搜索
    return search(root->right, key);
}

// 内部中序遍历方法
void Treap::inorderTraversal(TreapNode *node)
{
    if (node != nullptr)
    {
        inorderTraversal(node->left);
        std::cout << node->key << "(" << node->priority << ") ";
        inorderTraversal(node->right);
    }
}

// 打印树结构
void Treap::printStructure(TreapNode *node, std::string prefix, bool isLeft)
{
    if (node == nullptr)
        return;

    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << node->key << "(" << node->priority << ")" << std::endl;

    printStructure(node->left, prefix + (isLeft ? "│   " : "    "), true);
    printStructure(node->right, prefix + (isLeft ? "│   " : "    "), false);
}

// 公共接口：插入
void Treap::insert(int key)
{
    root = insert(root, key);
}

// 公共接口：删除
void Treap::remove(int key)
{
    root = remove(root, key);
}

// 公共接口：搜索
bool Treap::search(int key)
{
    return search(root, key) != nullptr;
}

// 公共接口：中序遍历
void Treap::inorder()
{
    inorderTraversal(root);
    std::cout << std::endl;
}

// 公共接口：打印树结构
void Treap::printTree()
{
    std::cout << "Treap结构：" << std::endl;
    printStructure(root, "", false);
}

// 内部方法：范围查询
void Treap::getKeysInRange(TreapNode *node, int start, int end, std::vector<int> &result)
{
    if (node == nullptr)
        return;

    // 如果当前节点键值大于等于start，需要搜索左子树
    if (start < node->key)
        getKeysInRange(node->left, start, end, result);

    // 如果当前节点键值在范围内，加入结果
    if (start <= node->key && node->key <= end)
        result.push_back(node->key);

    // 如果当前节点键值小于等于end，需要搜索右子树
    if (node->key < end)
        getKeysInRange(node->right, start, end, result);
}

// 公共接口：范围查询
std::vector<int> Treap::getKeysInRange(int start, int end)
{
    std::vector<int> result;
    getKeysInRange(root, start, end, result);
    return result;
}

int main()
{
    // 创建Treap
    Treap treap;

    // 插入一些值
    treap.insert(50);
    treap.insert(30);
    treap.insert(70);
    treap.insert(20);
    treap.insert(40);
    treap.insert(60);
    treap.insert(80);

    // 打印树结构
    std::cout << "初始Treap结构：" << std::endl;
    treap.printTree();

    // 中序遍历（按键值排序输出）
    std::cout << "中序遍历: ";
    treap.inorder();

    // 搜索
    int searchKey = 40;
    std::cout << "搜索键值 " << searchKey << ": "
              << (treap.search(searchKey) ? "找到" : "未找到") << std::endl;

    // 范围查询
    int start = 30, end = 70;
    std::vector<int> rangeResult = treap.getKeysInRange(start, end);

    std::cout << "范围 [" << start << ", " << end << "] 内的键值: ";
    for (int key : rangeResult)
    {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    // 删除节点
    int removeKey = 30;
    std::cout << "删除键值 " << removeKey << std::endl;
    treap.remove(removeKey);

    // 打印删除后的树结构
    std::cout << "删除后的Treap结构：" << std::endl;
    treap.printTree();

    // 再次中序遍历
    std::cout << "删除后的中序遍历: ";
    treap.inorder();

    return 0;
}