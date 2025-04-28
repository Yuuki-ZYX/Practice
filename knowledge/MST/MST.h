#ifndef MST_H
#define MST_H

#include "graph.h"
#include <vector>
#include <queue>
#include <map>

// 用于优先队列的比较结构 (Prim算法)
struct ComparePrim
{
    bool operator()(const std::pair<double, int> &a, const std::pair<double, int> &b)
    {
        return a.first > b.first; // 最小堆，权重小的优先
    }
};

// 最小生成树类
class MST
{
private:
    const Graph &graph;      // 图的引用
    std::vector<int> parent; // MST中的父节点
    std::vector<double> key; // 连接顶点到MST的最小边的权重
    std::vector<bool> inMST; // 顶点是否已包含在MST中
    double totalWeight;      // MST的总权重

    // 内部执行Prim算法
    void runPrim(int startNode);

public:
    MST(const Graph &g);
    ~MST();

    double getTotalWeight() const;
    void printMSTEdges() const;
    std::vector<std::pair<int, int>> getMSTEdges() const; // 返回边列表 {u, v}
};

#endif // MST_H