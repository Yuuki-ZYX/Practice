#include <iostream>
#include <vector>
#include <queue>

void DFS_helper(int u, const std::vector<std::vector<int>> &adj, std::vector<bool> &visited)
{
    visited[u] = true;
    std::cout << u << " ";

    for (const auto &v : adj[u])
    {
        if (!visited[v])
        {
            DFS_helper(v, adj, visited);
        }
    }
}

void DFS(int n, const std::vector<std::vector<int>> &adj)
{
    std::vector<bool> visited(n, false);

    std::cout << "DFS Order: ";
    for (int i = 0; i < n; i++)
    {
        if (visited[i])
            continue;
        DFS_helper(i, adj, visited);
    }
    std::cout << std::endl;
}

void BFS(int n, const std::vector<std::vector<int>> &adj)
{
    std::vector<bool> visited(n, false);
    std::cout << "BFS Order: ";

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            std::queue<int> q;

            visited[i] = true;
            q.push(i);
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                std::cout << u << " ";

                for (const auto &v : adj[u])
                {
                    if (!visited[v])
                    {
                        q.push(v);
                        visited[v] = true;
                    }
                }
            }
        }
    }
    std::cout << std::endl;
}

int main()
{
    int n = 9;
    std::vector<std::vector<int>> adj(n);
    // 添加边 (创建图)
    auto add_edge = [&](int u, int v)
    {
        adj[u].push_back(v);
        // 如果是无向图，也添加反向边
        adj[v].push_back(u);
    };
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(2, 6);
    add_edge(7, 8); // 添加另一个组件

    std::cout << "Graph Adjacency List:" << std::endl;
    for (int i = 0; i < n; ++i)
    {
        std::cout << i << ": ";
        for (int neighbor : adj[i])
        {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 执行 DFS
    DFS(n, adj);

    // 执行 BFS
    BFS(n, adj);

    return 0;
}