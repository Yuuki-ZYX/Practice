#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

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

// Dijkstra 算法函数
// adj: 图的邻接表表示，adj[i] 存储从顶点 i 出发的所有边
// n:   图的顶点数量 (顶点编号从 0 到 n-1)
// start_node: 起始顶点
std::vector<int> dijkstra(const std::vector<std::vector<Edge>> adj, int n, int start_node)
{
    std::vector<int> dist(n, INF);

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
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main()
{
    int n = 6;
    std::vector<std::vector<Edge>> adj(n);
    auto add_edge = [&](int u, int v, int w)
    {
        adj[u].push_back({v, w});
    };

    // add_edge(0, 1, 7);
    add_edge(0, 2, 9);
    add_edge(0, 5, 14);
    add_edge(1, 2, 10);
    add_edge(1, 3, 15);
    add_edge(2, 3, 11);
    add_edge(2, 5, 2);
    add_edge(3, 4, 6);
    add_edge(4, 5, 9);

    int start_node = 0;

    std::vector<int> shortest_distances = dijkstra(adj, n, start_node);

    std::cout << "Shortest distances from node " << start_node << ":" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << "Node " << i << ": ";
        if (shortest_distances[i] == INF)
        {
            std::cout << "INF" << std::endl;
        }
        else
        {
            std::cout << shortest_distances[i] << std::endl;
        }
    }
    return 0;
}