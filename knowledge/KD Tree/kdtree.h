#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <limits>
#include <cmath>

// KD树节点类
class KDNode
{
public:
    std::vector<double> point; // k维空间中的点
    KDNode *left;
    KDNode *right;

    explicit KDNode(const std::vector<double> &p);
};

// KD树类
class KDTree
{
private:
    KDNode *root;
    int dimensions; // 维度数量

    // 工具方法
    KDNode *insert(KDNode *node, const std::vector<double> &point, int depth);
    bool search(KDNode *node, const std::vector<double> &point, int depth);
    KDNode *findNearest(KDNode *node, const std::vector<double> &target, KDNode *best, double &bestDist, int depth);
    void rangeSearch(KDNode *node, const std::vector<double> &min, const std::vector<double> &max, std::vector<std::vector<double>> &result, int depth);
    void destroyTree(KDNode *node);
    double distance(const std::vector<double> &p1, const std::vector<double> &p2);
    void inOrderTraversal(KDNode *node);

public:
    KDTree(int k);
    ~KDTree();

    void insert(const std::vector<double> &point);
    bool contains(const std::vector<double> &point);
    std::vector<double> findNearest(const std::vector<double> &target);
    std::vector<std::vector<double>> rangeSearch(const std::vector<double> &min, const std::vector<double> &max);
    void inOrder();
};

#endif // KDTREE_H