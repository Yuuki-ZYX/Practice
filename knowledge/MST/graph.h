#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <limits>

const double INF = std::numeric_limits<double>::infinity();

// 边结构
struct Edge
{
    int to;        // 边的终点
    double weight; // 边的权重

    Edge(int t, double w) : to(t), weight(w) {}
};

// 图类 (使用邻接表表示)
class Graph
{
public:
    int numVertices;                        // 顶点数量
    std::vector<std::vector<Edge>> adjList; // 邻接表

    Graph(int n) : numVertices(n), adjList(n) {}

    // 添加无向边 (用于MST)
    void addUndirectedEdge(int u, int v, double weight)
    {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices)
        {
            adjList[u].emplace_back(v, weight);
            adjList[v].emplace_back(u, weight); // 无向图，双向添加
        }
    }

    // 添加有向边 (用于SPT)
    void addDirectedEdge(int u, int v, double weight)
    {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices)
        {
            adjList[u].emplace_back(v, weight);
        }
    }
};

#endif // GRAPH_H