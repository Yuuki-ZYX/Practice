#include <iostream>
#include <vector>
#include <limits>
#include <algorithm> // 包含 min 函数

// 定义无穷大
const int INF = std::numeric_limits<int>::max() / 2; // 除以 2 防止加法溢出

// Floyd-Warshall 算法函数
// adj_matrix: 图的邻接矩阵表示。adj_matrix[i][j] 存储从顶点 i 到顶点 j 的边的权重。
//             如果 i 和 j 之间没有直接边，则存储 INF。对角线元素 adj_matrix[i][i] 应为 0。
// n:          图的顶点数量 (顶点编号从 0 到 n-1)
// 返回值:     一个 n x n 的矩阵，其中 result[i][j] 存储从顶点 i 到顶点 j 的最短路径长度。
std::vector<std::vector<int>> floyd_warshall(std::vector<std::vector<int>> &dist, int n)
{
    // dist 矩阵在传入时已经初始化：
    // dist[i][i] = 0
    // dist[i][j] = weight(i, j) 如果存在直接边
    // dist[i][j] = INF 如果不存在直接边

    // 核心思想：尝试通过每一个可能的中间顶点 k 来更新任意两个顶点 i 和 j 之间的最短距离。
    // k 是最外层循环，表示我们允许使用的中间顶点的最大编号。
    // 也就是说，当 k = 0 时，我们只考虑是否可以通过顶点 0 来缩短 i 和 j 之间的距离。
    // 当 k = 1 时，我们考虑是否可以通过顶点 0 或 1 来缩短 i 和 j 之间的距离。
    // ... 以此类推，直到 k = n-1，此时我们考虑了所有顶点作为中间顶点的情况。
    for (int k = 0; k < n; ++k)
    {
        // 遍历所有可能的起始顶点 i
        for (int i = 0; i < n; ++i)
        {
            // 遍历所有可能的结束顶点 j
            for (int j = 0; j < n; ++j)
            {
                // 检查从 i 到 k 和从 k 到 j 的路径是否存在（不是 INF）
                // 这是为了防止 INF + weight 导致的整数溢出或错误的距离计算
                if (dist[i][k] != INF && dist[k][j] != INF)
                {
                    // 状态转移方程：
                    // dist[i][j] 的当前值表示不经过顶点 k（或经过编号小于 k 的顶点）时，从 i 到 j 的最短距离。
                    // dist[i][k] + dist[k][j] 表示经过顶点 k 时，从 i 到 j 的路径长度。
                    // 我们比较这两者，取较小值作为新的 dist[i][j]，即更新后的最短距离。
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        // 经过第 k 轮迭代后，dist[i][j] 存储的是从 i 到 j，只允许使用编号从 0 到 k 的顶点作为中间节点时的最短路径长度。
    }

    // 当所有 k 都迭代完毕后，dist[i][j] 就存储了从 i 到 j 的全局最短路径长度，允许使用任何顶点作为中间节点。
    return dist;
}

int main()
{
    int n = 6; // 顶点数量

    // 初始化邻接矩阵
    // dist[i][j] 表示从 i 到 j 的直接距离
    // 对角线为 0，没有直接连接的边设为 INF
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    for (int i = 0; i < n; ++i)
    {
        dist[i][i] = 0;
    }

    // 添加边 (u, v, weight)
    auto add_edge = [&](int u, int v, int w)
    {
        // 假设是无向图，如果是有向图，则只设置 dist[u][v] = w;
        dist[u][v] = w;
        // dist[v][u] = w; // 如果是无向图，取消注释此行
    };

    // add_edge(0, 1, 7); // 假设顶点 0 和 1 之间没有直接边
    add_edge(0, 2, 9);
    add_edge(0, 5, 14);
    add_edge(1, 2, 10);
    add_edge(1, 3, 15);
    add_edge(2, 3, 11);
    add_edge(2, 5, 2);
    add_edge(3, 4, 6);
    add_edge(4, 5, 9);
    // 注意：Floyd-Warshall 通常用于稠密图，或者需要计算所有点对最短路径的情况。
    // 对于稀疏图且只需要单源最短路径，Dijkstra 通常更高效。

    // 调用 Floyd-Warshall 算法
    std::vector<std::vector<int>> shortest_paths = floyd_warshall(dist, n);

    // 打印所有顶点对之间的最短距离
    std::cout << "Shortest distances between all pairs of nodes:" << std::endl;
    std::cout << "   ";
    for (int j = 0; j < n; ++j)
    {
        printf("%5d", j); // 打印列标题
    }
    std::cout << std::endl;
    std::cout << "------------------------------------" << std::endl;

    for (int i = 0; i < n; ++i)
    {
        printf("%2d|", i); // 打印行标题
        for (int j = 0; j < n; ++j)
        {
            if (shortest_paths[i][j] == INF)
            {
                printf("%5s", "INF");
            }
            else
            {
                printf("%5d", shortest_paths[i][j]);
            }
        }
        std::cout << std::endl;
    }

    return 0;
}