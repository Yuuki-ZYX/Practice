#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <algorithm>

// 定义无穷大
const int INF = std::numeric_limits<int>::max();

struct Edge
{
    int to;
    int weight;
};

// 定义优先队列中存储的元素类型 {distance, vertex}
// 使用 pair，第一个元素是距离，第二个元素是顶点编号
// 这样优先队列会默认按距离从小到大排序
using P = std::pair<int, int>;

// Dijkstra 算法结果的结构体
struct DijkstraResult
{
    std::vector<int> dist;   // 最短路径数组
    std::vector<int> parent; // 前驱节点数组
};

// Dijkstra 算法函数
// adj: 图的邻接表表示，adj[i] 存储从顶点 i 出发的所有边
// n:   图的顶点数量 (顶点编号从 0 到 n-1)
// start_node: 起始顶点
DijkstraResult dijkstra(const std::vector<std::vector<Edge>> adj, int n, int start_node)
{
    std::vector<int> dist(n, INF);
    std::vector<int> parent(n, -1);

    dist[start_node] = 0;

    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    pq.push({0, start_node});

    while (!pq.empty())
    {
        P current = pq.top();
        pq.pop();

        int d = current.first;
        int u = current.second;

        if (d > dist[u]) // d 一定是 大于等于 dist[u]的
            continue;    // 说明取出来的是被舍弃的列的旧值，忽视

        for (const auto &edge : adj[u])
        {
            int v = edge.to;
            int w = edge.weight;

            if (dist[u] + w < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, parent};
}

std::vector<int> getPath(const std::vector<int> &parent, int start_node, int end_node)
{
    std::vector<int> path;
    if (parent[end_node] == -1 && end_node != start_node)
    {
        return path; // 如果 end_node 不可达 (且不是 start_node 本身)，返回空路径
    }
    // 从 end_node 回溯直到 start_node
    for (int current = end_node; current != -1; current = parent[current])
    {
        path.push_back(current);
    }
    // 因为是从后往前添加的，所以需要反转路径
    std::reverse(path.begin(), path.end());
    return path;
}

int main()
{
    int n = 6;
    std::vector<std::vector<Edge>> adj(n);
    auto add_edge = [&](int u, int v, int w)
    {
        adj[u].push_back({v, w});
    };

    add_edge(0, 1, 7);
    add_edge(0, 2, 9);
    add_edge(0, 5, 14);
    add_edge(1, 2, 10);
    add_edge(1, 3, 15);
    add_edge(2, 3, 11);
    add_edge(2, 5, 2);
    add_edge(3, 4, 6);
    add_edge(4, 5, 9);

    int start_node = 0;

    DijkstraResult res = dijkstra(adj, n, start_node);
    const auto &shortest_distances = res.dist;
    const auto &parents = res.parent;

    std::cout << "Shortest paths from node " << start_node << ":" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << "Node " << i << ": ";
        if (shortest_distances[i] == INF)
        {
            std::cout << "INF (No path)" << std::endl;
        }
        else
        {
            std::cout << "Distance = " << shortest_distances[i] << ", Path: ";
            std::vector<int> path = getPath(parents, start_node, i);
            if (path.empty() && i != start_node)
            { // 再次检查路径是否为空
                std::cout << "(Path reconstruction failed or node unreachable)";
            }
            else if (path.empty() && i == start_node)
            { // 处理起点就是终点的情况
                std::cout << start_node;
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
    return 0;
}