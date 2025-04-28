#ifndef SPT_H
#define SPT_H

#include "graph.h"
#include <vector>
#include <queue>
#include <map>

// 用于优先队列的比较结构
struct CompareDist
{
    bool operator()(const std::pair<double, int> &a, const std::pair<double, int> &b)
    {
        return a.first > b.first; // 最小堆，距离小的优先
    }
};

// 最短路径树类
class SPT
{
private:
    int source;               // 源顶点
    std::vector<double> dist; // 从源点到各顶点的最短距离
    std::vector<int> parent;  // 最短路径树中的父节点
    const Graph &graph;       // 图的引用

    // 内部执行Dijkstra算法
    void runDijkstra();

public:
    SPT(const Graph &g, int src);
    ~SPT();

    double getDistance(int destination) const;
    std::vector<int> getPath(int destination) const;
    void printDistances() const;
    void printPaths() const;
};

#endif // SPT_H