#include "mst.h"
#include <iostream>
#include <vector>
#include <queue>

// MST构造函数
MST::MST(const Graph &g) : graph(g), totalWeight(0.0)
{
    if (graph.numVertices == 0)
        return;

    parent.assign(graph.numVertices, -1);
    key.assign(graph.numVertices, INF);
    inMST.assign(graph.numVertices, false);

    // 从顶点0开始构建MST
    runPrim(0);

    // 计算总权重 (也可以在Prim算法中累加)
    for (int i = 0; i < graph.numVertices; ++i)
    {
        if (parent[i] != -1)
        {
            // 查找连接 parent[i] 和 i 的边的权重
            for (const auto &edge : graph.adjList[i])
            {
                if (edge.to == parent[i])
                {
                    totalWeight += edge.weight;
                    break;
                }
            }
        }
        else if (i != 0 && key[i] == INF)
        { // 处理图不连通的情况
            std::cerr << "警告：图可能不连通，无法形成覆盖所有顶点的MST" << std::endl;
            totalWeight = INF; // 标记为无穷大
            break;
        }
    }
    // 如果图只有一个节点
    if (graph.numVertices == 1)
        totalWeight = 0.0;
}

// MST析构函数
MST::~MST() {}

// 内部执行Prim算法
void MST::runPrim(int startNode)
{
    // 优先队列存储 {最小连接权重, 顶点}
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, ComparePrim> pq;

    key[startNode] = 0.0; // 起始点的key为0
    pq.push({0.0, startNode});

    while (!pq.empty())
    {
        double k = pq.top().first; // 当前最小key值 (权重)
        int u = pq.top().second;   // 对应的顶点
        pq.pop();

        // 如果顶点u已经被包含在MST中，则跳过
        if (inMST[u])
            continue;

        inMST[u] = true; // 将顶点u加入MST

        // 遍历顶点u的所有邻居v
        for (const Edge &edge : graph.adjList[u])
        {
            int v = edge.to;
            double weight = edge.weight;

            // 如果v不在MST中，并且通过u连接v的边的权重小于v当前的key值
            if (!inMST[v] && weight < key[v])
            {
                key[v] = weight;      // 更新v的最小连接权重
                parent[v] = u;        // 设置v在MST中的父节点为u
                pq.push({key[v], v}); // 将{新权重, v}加入优先队列
            }
        }
    }
}

// 获取MST的总权重
double MST::getTotalWeight() const
{
    return totalWeight;
}

// 获取构成MST的边列表
std::vector<std::pair<int, int>> MST::getMSTEdges() const
{
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < graph.numVertices; ++i)
    {
        if (parent[i] != -1)
        {
            // 添加边 {父节点, 当前节点}
            edges.push_back({parent[i], i});
        }
    }
    return edges;
}

// 打印构成MST的边及其权重
void MST::printMSTEdges() const
{
    if (totalWeight == INF)
    {
        std::cout << "图不连通，无法打印MST边。" << std::endl;
        return;
    }
    std::cout << "最小生成树 (MST) 的边:" << std::endl;
    double calculated_weight = 0;
    for (int i = 0; i < graph.numVertices; ++i)
    {
        if (parent[i] != -1)
        {
            double edge_weight = 0;
            // 查找连接 parent[i] 和 i 的边的权重
            for (const auto &edge : graph.adjList[i])
            {
                if (edge.to == parent[i])
                {
                    edge_weight = edge.weight;
                    break;
                }
            }
            std::cout << "  边 (" << parent[i] << " - " << i << ") 权重: " << edge_weight << std::endl;
            calculated_weight += edge_weight;
        }
    }
    std::cout << "计算得到的总权重: " << calculated_weight << std::endl;
}

// 测试MST的示例main函数
int main_mst()
{
    // 创建图 (无向图)
    Graph g(5);
    g.addUndirectedEdge(0, 1, 2);
    g.addUndirectedEdge(0, 3, 6);
    g.addUndirectedEdge(1, 2, 3);
    g.addUndirectedEdge(1, 3, 8);
    g.addUndirectedEdge(1, 4, 5);
    g.addUndirectedEdge(2, 4, 7);
    g.addUndirectedEdge(3, 4, 9);

    MST mst(g);

    std::cout << "--- 最小生成树 (Prim) ---" << std::endl;
    mst.printMSTEdges();
    std::cout << "MST 总权重: " << mst.getTotalWeight() << std::endl;

    return 0;
}

// 如果需要独立运行，可以取消下面的注释
int main()
{
    std::cout << "\n运行 MST 测试..." << std::endl;
    main_mst();
    return 0;
}