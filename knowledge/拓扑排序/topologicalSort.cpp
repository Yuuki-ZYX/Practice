#include <iostream>
#include <vector>
#include <queue>
#include <vector>

using Edge = std::pair<int, int>;

std::vector<int> topologicalSort(int n, const std::vector<Edge> &edges)
{
    // 1.构建邻接表和计算入度
    std::vector<std::vector<int>> adj(n);
    std::vector<int> in_degree(n, 0);
    for (const auto &edge : edges)
    {
        int u = edge.first;
        int v = edge.second;

        adj[u].push_back(v);
        in_degree[v]++;
    }

    // 2.初始化队列，把所有入度为0的点入队
    std::queue<int> q;
    for (int i = 0; i < n; i++)
    {
        if (in_degree[i] == 0)
        {
            q.push(i);
        }
    }

    // 3.处理队伍中的顶点
    std::vector<int> res;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        res.push_back(u);

        for (const auto &v : adj[u])
        {
            in_degree[v]--;
            if (in_degree[v] == 0)
                q.push(v);
        }
    }
    // 4.检查是否有环
    //  如果结果列表中的顶点数量不等于总顶点数，说明图中存在环
    if (res.size() != n)
    {
        // 返回空列表表示存在环，无法进行拓扑排序
        return {};
    }
    return res;
}

int main()
{
    // 示例：创建一个有向无环图 (DAG)
    int n = 6; // 顶点数量 (0 to 5)
    std::vector<std::pair<int, int>> edges = {
        {5, 2}, {5, 0}, {4, 0}, {4, 1}, {2, 3}, {3, 1}};
    // 预期拓扑排序的一种可能结果: 4, 5, 0, 2, 3, 1 (或其他有效顺序)

    std::vector<int> sorted_order = topologicalSort(n, edges);

    if (sorted_order.empty())
    {
        std::cout << "Graph contains a cycle, topological sort not possible." << std::endl;
    }
    else
    {
        std::cout << "Topological Sort Order:" << std::endl;
        for (size_t i = 0; i < sorted_order.size(); ++i)
        {
            std::cout << sorted_order[i] << (i == sorted_order.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    }

    // 示例：创建一个带环的图
    std::cout << "\nTesting graph with a cycle:" << std::endl;
    std::vector<std::pair<int, int>> cyclic_edges = {
        {0, 1}, {1, 2}, {2, 0} // 0 -> 1 -> 2 -> 0 形成环
    };
    int n_cyclic = 3;
    std::vector<int> cyclic_sorted_order = topologicalSort(n_cyclic, cyclic_edges);
    if (cyclic_sorted_order.empty())
    {
        std::cout << "Graph contains a cycle, topological sort not possible." << std::endl;
    }
    else
    {
        std::cout << "Topological Sort Order:" << std::endl;
        for (size_t i = 0; i < cyclic_sorted_order.size(); ++i)
        {
            std::cout << cyclic_sorted_order[i] << (i == cyclic_sorted_order.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    }

    return 0;
}