#include "kdtree.h"
#include <iostream>
#include <algorithm>

// 节点构造函数
KDNode::KDNode(const std::vector<double> &p) : point(p), left(nullptr), right(nullptr) {}

// KD树构造函数
KDTree::KDTree(int k) : root(nullptr), dimensions(k) {}

// 析构函数
KDTree::~KDTree()
{
    destroyTree(root);
}

// 递归释放节点
void KDTree::destroyTree(KDNode *node)
{
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// 计算两点之间的欧几里得距离
double KDTree::distance(const std::vector<double> &p1, const std::vector<double> &p2)
{
    double dist = 0.0;
    for (size_t i = 0; i < p1.size(); ++i)
    {
        double diff = p1[i] - p2[i];
        dist += diff * diff;
    }
    return dist;
}

// 内部插入方法（递归）
KDNode *KDTree::insert(KDNode *node, const std::vector<double> &point, int depth)
{
    // 如果树为空，创建新节点
    if (node == nullptr)
        return new KDNode(point);

    // 计算当前级别的维度
    int cd = depth % dimensions;

    // 根据当前维度决定向左还是向右
    if (point[cd] < node->point[cd])
        node->left = insert(node->left, point, depth + 1);
    else
        node->right = insert(node->right, point, depth + 1);

    return node;
}

// 内部搜索方法（递归）
bool KDTree::search(KDNode *node, const std::vector<double> &point, int depth)
{
    if (node == nullptr)
        return false;

    // 检查当前节点是否匹配
    bool isMatch = true;
    for (int i = 0; i < dimensions; ++i)
    {
        if (node->point[i] != point[i])
        {
            isMatch = false;
            break;
        }
    }

    if (isMatch)
        return true;

    // 计算当前级别的维度
    int cd = depth % dimensions;

    // 根据当前维度决定向左还是向右
    if (point[cd] < node->point[cd])
        return search(node->left, point, depth + 1);
    else
        return search(node->right, point, depth + 1);
}

// 内部最近邻搜索方法（递归）
KDNode *KDTree::findNearest(KDNode *node, const std::vector<double> &target, KDNode *best, double &bestDist, int depth)
{
    if (node == nullptr)
        return best;

    // 计算当前点到目标的距离
    double dist = distance(node->point, target);

    // 更新最近点
    if (dist < bestDist)
    {
        bestDist = dist;
        best = node;
    }

    // 计算当前级别的维度
    int cd = depth % dimensions;

    // 优先搜索可能包含更近点的子树
    KDNode *first = nullptr;
    KDNode *second = nullptr;

    if (target[cd] < node->point[cd])
    {
        first = node->left;
        second = node->right;
    }
    else
    {
        first = node->right;
        second = node->left;
    }

    // 递归搜索第一个子树
    best = findNearest(first, target, best, bestDist, depth + 1);

    // 如果超平面与超球体相交，则搜索另一个子树
    double axisDistance = target[cd] - node->point[cd];
    if (axisDistance * axisDistance < bestDist)
    {
        best = findNearest(second, target, best, bestDist, depth + 1);
    }

    return best;
}

// 内部范围搜索方法（递归）
void KDTree::rangeSearch(KDNode *node, const std::vector<double> &min, const std::vector<double> &max, std::vector<std::vector<double>> &result, int depth)
{
    if (node == nullptr)
        return;

    // 检查当前点是否在范围内
    bool inRange = true;
    for (int i = 0; i < dimensions; ++i)
    {
        if (node->point[i] < min[i] || node->point[i] > max[i])
        {
            inRange = false;
            break;
        }
    }

    if (inRange)
        result.push_back(node->point);

    // 计算当前级别的维度
    int cd = depth % dimensions;

    // 如果范围的左边界小于当前节点，则搜索左子树
    if (min[cd] <= node->point[cd])
        rangeSearch(node->left, min, max, result, depth + 1);

    // 如果范围的右边界大于当前节点，则搜索右子树
    if (max[cd] >= node->point[cd])
        rangeSearch(node->right, min, max, result, depth + 1);
}

// 中序遍历辅助函数
void KDTree::inOrderTraversal(KDNode *node)
{
    if (node != nullptr)
    {
        inOrderTraversal(node->left);

        std::cout << "(";
        for (size_t i = 0; i < node->point.size(); ++i)
        {
            std::cout << node->point[i];
            if (i < node->point.size() - 1)
                std::cout << ", ";
        }
        std::cout << ") ";

        inOrderTraversal(node->right);
    }
}

// 公共接口：插入
void KDTree::insert(const std::vector<double> &point)
{
    if (point.size() != dimensions)
    {
        std::cerr << "错误：点的维度不匹配" << std::endl;
        return;
    }
    root = insert(root, point, 0);
}

// 公共接口：检查是否包含某点
bool KDTree::contains(const std::vector<double> &point)
{
    if (point.size() != dimensions)
    {
        std::cerr << "错误：点的维度不匹配" << std::endl;
        return false;
    }
    return search(root, point, 0);
}

// 公共接口：最近邻搜索
std::vector<double> KDTree::findNearest(const std::vector<double> &target)
{
    if (target.size() != dimensions)
    {
        std::cerr << "错误：点的维度不匹配" << std::endl;
        return std::vector<double>();
    }

    if (root == nullptr)
        return std::vector<double>();

    double bestDist = std::numeric_limits<double>::max();
    KDNode *best = nullptr;

    best = findNearest(root, target, best, bestDist, 0);

    return best ? best->point : std::vector<double>();
}

// 公共接口：范围搜索
std::vector<std::vector<double>> KDTree::rangeSearch(const std::vector<double> &min, const std::vector<double> &max)
{
    if (min.size() != dimensions || max.size() != dimensions)
    {
        std::cerr << "错误：范围的维度不匹配" << std::endl;
        return std::vector<std::vector<double>>();
    }

    std::vector<std::vector<double>> result;
    rangeSearch(root, min, max, result, 0);
    return result;
}

// 公共接口：中序遍历
void KDTree::inOrder()
{
    inOrderTraversal(root);
    std::cout << std::endl;
}

// 测试KD树的示例main函数
int main()
{
    // 创建2维KD树
    KDTree kdtree(2);

    // 插入一些点
    kdtree.insert({3.0, 6.0});
    kdtree.insert({17.0, 15.0});
    kdtree.insert({13.0, 15.0});
    kdtree.insert({6.0, 12.0});
    kdtree.insert({9.0, 1.0});
    kdtree.insert({2.0, 7.0});
    kdtree.insert({10.0, 19.0});

    std::cout << "中序遍历所有点: ";
    kdtree.inOrder();

    // 查找点
    std::vector<double> searchPoint = {13.0, 15.0};
    std::cout << "包含点 (13, 15)? " << (kdtree.contains(searchPoint) ? "是" : "否") << std::endl;

    // 最近邻搜索
    std::vector<double> targetPoint = {12.0, 10.0};
    std::vector<double> nearest = kdtree.findNearest(targetPoint);

    std::cout << "最接近 (12, 10) 的点是: (";
    for (size_t i = 0; i < nearest.size(); ++i)
    {
        std::cout << nearest[i];
        if (i < nearest.size() - 1)
            std::cout << ", ";
    }
    std::cout << ")" << std::endl;

    // 范围搜索
    std::vector<double> minRange = {5.0, 5.0};
    std::vector<double> maxRange = {15.0, 20.0};
    std::vector<std::vector<double>> rangeResults = kdtree.rangeSearch(minRange, maxRange);

    std::cout << "在范围 ([5,5] 到 [15,20]) 内的点: " << std::endl;
    for (const auto &point : rangeResults)
    {
        std::cout << "(";
        for (size_t i = 0; i < point.size(); ++i)
        {
            std::cout << point[i];
            if (i < point.size() - 1)
                std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }

    return 0;
}