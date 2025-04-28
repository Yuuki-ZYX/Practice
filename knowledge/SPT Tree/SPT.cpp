#include "spt.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // for std::reverse

// SPT构造函数
SPT::SPT(const Graph &g, int src) : graph(g), source(src)
{
    if (source < 0 || source >= graph.numVertices)
    {
        std::cerr << "错误：源顶点索引无效" << std::endl;
        return;
    }
    dist.assign(graph.numVertices, INF);
    parent.assign(graph.numVertices, -1); // -1 表示没有父节点
    runDijkstra();
}

// SPT析构函数
SPT::~SPT() {}

// 内部执行Dijkstra算法
void SPT::runDijkstra()
{
    dist[source] = 0.0;
    // 优先队列存储 {距离, 顶点}
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, CompareDist> pq;
    pq.push({0.0, source});

    while (!pq.empty())
    {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // 如果找到了更短的路径，则忽略当前路径
        if (d > dist[u])
            continue;

        // 遍历顶点u的所有邻居v
        for (const Edge &edge : graph.adjList[u])
        {
            int v = edge.to;
            double weight = edge.weight;

            // 松弛操作：如果通过u到达v的路径更短
            if (dist[u] != INF && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// 获取到目标顶点的最短距离
double SPT::getDistance(int destination) const
{
    if (destination < 0 || destination >= graph.numVertices)
    {
        std::cerr << "错误：目标顶点索引无效" << std::endl;
        return INF;
    }
    return dist[destination];
}

// 获取到目标顶点的最短路径
std::vector<int> SPT::getPath(int destination) const
{
    std::vector<int> path;
    if (destination < 0 || destination >= graph.numVertices || dist[destination] == INF)
    {
        return path; // 目标不可达或索引无效
    }

    for (int current = destination; current != -1; current = parent[current])
    {
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end()); // 反转得到从源到目标的路径
    return path;
}

// 打印所有最短距离
void SPT::printDistances() const
{
    std::cout << "从源点 " << source << " 到各顶点的最短距离:" << std::endl;
    for (int i = 0; i < graph.numVertices; ++i)
    {
        std::cout << "  顶点 " << i << ": ";
        if (dist[i] == INF)
        {
            std::cout << "INF" << std::endl;
        }
        else
        {
            std::cout << dist[i] << std::endl;
        }
    }
}

// 打印所有最短路径
void SPT::printPaths() const
{
    std::cout << "从源点 " << source << " 到各顶点的最短路径:" << std::endl;
    for (int i = 0; i < graph.numVertices; ++i)
    {
        std::cout << "  顶点 " << i << ": ";
        std::vector<int> path = getPath(i);
        if (path.empty())
        {
            if (i == source)
            {
                std::cout << source;
            }
            else
            {
                std::cout << "不可达";
            }
        }
        else
        {
            for (size_t j = 0; j < path.size(); ++j)
            {
                std::cout << path[j] << (j == path.size() - 1 ? "" : " -> ");
            }
        }
        std::cout << std::endl;
    }
}

// 测试SPT的示例main函数
int main_spt()
{
    // 创建图 (有向图)
    Graph g(5);
    g.addDirectedEdge(0, 1, 10);
    g.addDirectedEdge(0, 3, 5);
    g.addDirectedEdge(1, 2, 1);
    g.addDirectedEdge(1, 3, 2);
    g.addDirectedEdge(2, 4, 4);
    g.addDirectedEdge(3, 1, 3);
    g.addDirectedEdge(3, 2, 9);
    g.addDirectedEdge(3, 4, 2);
    g.addDirectedEdge(4, 0, 7); // 注意环路
    g.addDirectedEdge(4, 2, 6);

    int sourceVertex = 0;
    SPT spt(g, sourceVertex);

    std::cout << "--- 最短路径树 (Dijkstra) ---" << std::endl;
    spt.printDistances();
    std::cout << std::endl;
    spt.printPaths();
    std::cout << std::endl;

    // 获取特定路径和距离
    int targetVertex = 4;
    std::cout << "到顶点 " << targetVertex << " 的最短距离: " << spt.getDistance(targetVertex) << std::endl;
    std::cout << "到顶点 " << targetVertex << " 的最短路径: ";
    std::vector<int> path = spt.getPath(targetVertex);
    for (size_t j = 0; j < path.size(); ++j)
    {
        std::cout << path[j] << (j == path.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;

    return 0;
}

int main()
{
    std::cout << "运行 SPT 测试..." << std::endl;
    main_spt();
    return 0;
}