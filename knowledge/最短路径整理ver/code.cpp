#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm> // std::min
#include <tuple>     // std::tuple for Bellman-Ford edges

// --- 常量定义 ---

// 定义长整型无穷大：选择一个足够大的值，同时避免加法溢出
const long long INF_LL = std::numeric_limits<long long>::max() / 3;
// 定义整型无穷大：同样避免加法溢出
const int INF_INT = std::numeric_limits<int>::max() / 3;

// --- 图的表示 ---

// 1. 邻接矩阵 (Adjacency Matrix):
//    - 适用于稠密图（边数接近 V^2）。
//    - Floyd-Warshall 算法通常使用邻接矩阵。
//    - 定义: std::vector<std::vector<int>> adj_matrix(V, std::vector<int>(V, INF_INT));
//    - adj_matrix[i][j] 存储从顶点 i 到顶点 j 的边的权重。若无直接边，则为 INF_INT。
//    - adj_matrix[i][i] 通常为 0。

// 2. 邻接表 (Adjacency List):
//    - 适用于稀疏图（边数远小于 V^2）。
//    - Dijkstra, Bellman-Ford, SPFA, BFS 通常使用邻接表。
//    - 定义: struct Edge { int to; int weight; };
//            std::vector<std::vector<Edge>> adj_list(V);
//    - adj_list[i] 存储所有从顶点 i 出发的边 (Edge)。

// 邻接表边结构体
struct Edge
{
    int to;     // 边的终点
    int weight; // 边的权重
};

// Dijkstra 算法优先队列元素类型 (最小堆)
// 存储 {距离, 顶点编号}，按距离排序
using PII = std::pair<long long, int>;

// Bellman-Ford 和 SPFA 需要存储所有边
struct EdgeInfo
{
    int u, v, w; // 起点, 终点, 权重
};

// --- 最短路径算法实现 ---

/**
 * @brief 广度优先搜索 (BFS) - 用于无权图
 *
 * @原理: 从源点开始，逐层扩展搜索。首先访问距离源点为 1 的所有顶点，然后是距离为 2 的，以此类推。
 *        保证找到的是边的数量最少的路径。
 *
 * @适用场景: 计算无权图中单源最短路径（路径长度指边的数量）。
 *
 * @复杂度: 时间 O(V + E)，空间 O(V)。其中 V 是顶点数，E 是边数。
 *
 * @param adj_list 图的邻接表表示 (边的权重会被忽略)。
 * @param n 顶点数量 (编号 0 到 n-1)。
 * @param start_node 源顶点。
 * @return std::vector<int> dist 数组，dist[i] 是从 start_node 到 i 的最短距离（边数），若不可达则为 INF_INT。
 */
std::vector<int> bfs(const std::vector<std::vector<Edge>> &adj_list, int n, int start_node)
{
    std::vector<int> dist(n, INF_INT);
    std::queue<int> q;

    if (start_node < 0 || start_node >= n)
        return dist; // 处理无效起始节点

    dist[start_node] = 0;
    q.push(start_node);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (const auto &edge : adj_list[u])
        {
            int v = edge.to;
            // 如果 v 尚未访问 (距离仍为 INF)
            if (dist[v] == INF_INT)
            {
                dist[v] = dist[u] + 1; // 距离是边的数量
                q.push(v);
            }
        }
    }
    return dist;
}

/**
 * @brief Dijkstra 算法 - 用于非负权图
 *
 * @原理: 贪心算法。维护一个已确定最短路径的顶点集合 S。初始时 S 只包含源点。
 *        每次从未访问的顶点中选择一个距离源点最近的顶点 u，将其加入 S。
 *        然后，更新（松弛）所有从 u 出发的边的终点 v 的距离：dist[v] = min(dist[v], dist[u] + weight(u, v))。
 *        使用优先队列（最小堆）可以高效地找到距离最近的未访问顶点。
 *
 * @适用场景: 计算带权图中单源最短路径，要求所有边的权重必须为 **非负数**。
 *
 * @复杂度:
 *        - 使用二叉堆实现的优先队列: 时间 O(E log V)，空间 O(V + E)。
 *        - 使用斐波那契堆: 时间 O(E + V log V)，空间 O(V + E)。
 *        在稀疏图中通常 O(E log V) 表现良好。
 *
 * @param adj_list 图的邻接表表示。
 * @param n 顶点数量 (编号 0 到 n-1)。
 * @param start_node 源顶点。
 * @return std::vector<long long> dist 数组，dist[i] 是从 start_node 到 i 的最短路径长度，若不可达则为 INF_LL。
 */
std::vector<long long> dijkstra(const std::vector<std::vector<Edge>> &adj_list, int n, int start_node)
{
    std::vector<long long> dist(n, INF_LL);
    // 优先队列，存储 <距离, 顶点>，按距离从小到大排序
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;

    if (start_node < 0 || start_node >= n)
        return dist; // 处理无效起始节点

    dist[start_node] = 0;
    pq.push({0, start_node});

    while (!pq.empty())
    {
        // 取出当前距离最小的顶点
        auto [d, u] = pq.top();
        pq.pop();

        // 如果取出的距离 d 比已记录的 dist[u] 大，说明这个是旧的、更长的路径信息，跳过
        if (d > dist[u])
        {
            continue;
        }

        // 松弛从 u 出发的所有边
        for (const auto &edge : adj_list[u])
        {
            int v = edge.to;
            int weight = edge.weight;
            // 如果通过 u 到达 v 的路径更短
            if (dist[u] != INF_LL && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v}); // 将更新后的距离和顶点加入优先队列
            }
        }
    }
    return dist;
}

/**
 * @brief Bellman-Ford 算法 - 用于带负权图（可检测负环）
 *
 * @原理: 对图中所有边进行 V-1 轮松弛操作。每一轮松弛操作都尝试更新所有边的终点的距离。
 *        核心思想是：从源点出发，最多经过 k 条边到达顶点 i 的最短路径，可以通过从源点出发，
 *        最多经过 k-1 条边到达某个顶点 j，然后通过边 (j, i) 到达 i 来获得。
 *        进行 V-1 轮后，如果没有负权环，就能保证找到所有最短路径。
 *        第 V 轮松弛：如果还能成功松弛某条边，说明存在从源点可达的负权环。
 *
 * @适用场景: 计算带权图中单源最短路径，可以处理 **负权边**。同时可以 **检测负权环**。
 *          复杂度较高，不如 Dijkstra 处理非负权图高效，也不如 SPFA 在某些负权图上快。
 *
 * @复杂度: 时间 O(V * E)，空间 O(V + E) 或 O(V)（取决于是否存储边列表）。
 *
 * @param edges 包含所有边的列表，每个元素是 {u, v, w}。
 * @param n 顶点数量 (编号 0 到 n-1)。
 * @param start_node 源顶点。
 * @return std::pair<std::vector<long long>, bool>
 *         - first: dist 数组，dist[i] 是从 start_node 到 i 的最短路径长度。
 *                  如果存在负环导致 i 的最短路无限小，可能为 -INF_LL 或某个很小的值，
 *                  或者在检测到负环后保持为 INF_LL（取决于具体实现）。
 *                  若不可达则为 INF_LL。
 *         - second: bool 值，true 表示检测到负权环，false 表示没有负权环。
 */
std::pair<std::vector<long long>, bool> bellman_ford(const std::vector<EdgeInfo> &edges, int n, int start_node)
{
    std::vector<long long> dist(n, INF_LL);
    bool has_negative_cycle = false;

    if (start_node < 0 || start_node >= n)
        return {dist, false}; // 处理无效起始节点

    dist[start_node] = 0;

    // 进行 V-1 轮松弛
    for (int i = 1; i < n; ++i)
    {
        bool updated = false;
        for (const auto &edge : edges)
        {
            int u = edge.u;
            int v = edge.v;
            int w = edge.w;
            if (dist[u] != INF_LL && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                updated = true;
            }
        }
        // 如果某一轮没有距离被更新，可以提前结束
        if (!updated)
            break;
    }

    // 第 V 轮检查负权环
    for (const auto &edge : edges)
    {
        int u = edge.u;
        int v = edge.v;
        int w = edge.w;
        if (dist[u] != INF_LL && dist[u] + w < dist[v])
        {
            has_negative_cycle = true;
            // 可以选择将受负环影响的点的距离设为负无穷，但这比较复杂
            // dist[v] = -INF_LL; // 标记受影响的点，可能需要传播
            break; // 检测到一个即可
        }
    }

    return {dist, has_negative_cycle};
}

/**
 * @brief SPFA (Shortest Path Faster Algorithm) - Bellman-Ford 的队列优化
 *
 * @原理: 类似于 Bellman-Ford，也是通过松弛操作更新距离。但它只将距离被成功缩短的顶点
 *        放入一个队列中，下一轮只对队列中顶点的出边进行松弛。这样避免了 Bellman-Ford
 *        中对很多不必要边的松弛操作。
 *        为了检测负环，记录每个顶点入队的次数，如果一个顶点入队次数超过 N 次，则存在负环。
 *
 * @适用场景: 计算带权图中单源最短路径，可以处理 **负权边**，并且可以 **检测负权环**。
 *          在稀疏图且没有负环或负环不多的情况下，通常比 Bellman-Ford 快得多，期望复杂度接近 O(E)。
 *          但在特定构造的图上（如网格图）可能退化到 O(V * E)。
 *
 * @复杂度:
 *        - 平均/期望时间: O(E) 或 O(kE) (k 是小常数)。
 *        - 最坏时间: O(V * E)。
 *        - 空间: O(V) 用于队列和距离/计数数组。
 *
 * @param adj_list 图的邻接表表示。
 * @param n 顶点数量 (编号 0 到 n-1)。
 * @param start_node 源顶点。
 * @return std::pair<std::vector<long long>, bool>
 *         - first: dist 数组，dist[i] 是从 start_node 到 i 的最短路径长度，若不可达则为 INF_LL。
 *                  如果存在负环，受影响的路径距离没有明确定义（可能保持 INF 或某个中间值）。
 *         - second: bool 值，true 表示检测到负权环，false 表示没有负权环。
 */
std::pair<std::vector<long long>, bool> spfa(const std::vector<std::vector<Edge>> &adj_list, int n, int start_node)
{
    std::vector<long long> dist(n, INF_LL);
    std::vector<int> count(n, 0);         // 记录每个顶点入队次数
    std::vector<bool> in_queue(n, false); // 标记顶点是否在队列中
    std::queue<int> q;
    bool has_negative_cycle = false;

    if (start_node < 0 || start_node >= n)
        return {dist, false}; // 处理无效起始节点

    dist[start_node] = 0;
    q.push(start_node);
    in_queue[start_node] = true;
    count[start_node]++;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        in_queue[u] = false; // 取出队列，标记不在队列中

        for (const auto &edge : adj_list[u])
        {
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] != INF_LL && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                // 如果 v 不在队列中，将其加入队列
                if (!in_queue[v])
                {
                    q.push(v);
                    in_queue[v] = true;
                    count[v]++;
                    // 如果一个顶点入队次数 >= n，说明存在负环
                    if (count[v] >= n)
                    {
                        has_negative_cycle = true;
                        // 可以立即返回，或者继续处理完当前队列以标记更多可能受影响的点
                        return {dist, true}; // 直接返回检测结果
                    }
                }
            }
        }
        // 如果在循环中检测到负环并返回，这里就不会执行
        // if (has_negative_cycle) break; // 如果选择在循环外判断，可以在这里跳出
    }

    return {dist, has_negative_cycle}; // 返回最终距离和是否检测到负环
}

/**
 * @brief Floyd-Warshall 算法 - 用于所有顶点对之间的最短路径
 *
 * @原理: 动态规划思想。设 dist[k][i][j] 表示从顶点 i 到顶点 j，只允许使用编号从 0 到 k
 *        的顶点作为中间节点时的最短路径长度。
 *        状态转移方程：dist[k][i][j] = min(dist[k-1][i][j], dist[k-1][i][k] + dist[k-1][k][j])
 *        含义是：从 i 到 j 经过 {0..k} 的最短路径，要么不经过 k（即 dist[k-1][i][j]），
 *        要么经过 k（即从 i 先到 k，再从 k 到 j，路径为 dist[k-1][i][k] + dist[k-1][k][j]）。
 *        实际实现中可以优化空间复杂度，省略 k 这一维，直接在原始距离矩阵上迭代更新。
 *        迭代顺序必须是 k 在最外层循环。
 *        检测负环：算法结束后，检查是否存在 dist[i][i] < 0。如果存在，则图中存在负权环。
 *
 * @适用场景: 计算图中 **所有顶点对** 之间的最短路径。可以处理 **带负权边** 的图，并能 **检测负权环**。
 *          适用于 **稠密图**，或者当需要所有顶点对最短路径信息时。对于稀疏图，多次运行
 *          Dijkstra（非负权）或 SPFA/Bellman-Ford（负权）可能更快。
 *
 * @复杂度: 时间 O(V^3)，空间 O(V^2) (使用邻接矩阵)。
 *
 * @param adj_matrix 初始邻接矩阵。adj_matrix[i][j] 是 i 到 j 的直接边权或 INF_INT。
 *                   adj_matrix[i][i] 必须为 0。函数会直接修改此矩阵。
 * @param n 顶点数量 (编号 0 到 n-1)。
 * @return std::pair<std::vector<std::vector<int>>, bool>
 *         - first: 更新后的邻接矩阵，存储所有顶点对之间的最短路径长度。
 *                  如果 i 到 j 的路径经过负环，dist[i][j] 的值没有保证（可能是负数）。
 *                  若不可达则为 INF_INT。
 *         - second: bool 值，true 表示检测到负权环，false 表示没有。
 */
std::pair<std::vector<std::vector<int>>, bool> floyd_warshall(std::vector<std::vector<int>> &dist, int n)
{
    // dist 矩阵在传入时应已初始化：
    // dist[i][i] = 0
    // dist[i][j] = weight(i, j) 如果存在直接边
    // dist[i][j] = INF_INT 如果不存在直接边

    // 核心 DP 过程
    for (int k = 0; k < n; ++k)
    { // 中间节点 k
        for (int i = 0; i < n; ++i)
        { // 起点 i
            for (int j = 0; j < n; ++j)
            { // 终点 j
                // 检查是否存在 i -> k 和 k -> j 的路径 (防止 INF + weight 溢出或错误计算)
                if (dist[i][k] != INF_INT && dist[k][j] != INF_INT)
                {
                    // 状态转移：尝试通过 k 来更新 i 到 j 的距离
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // 检测负权环
    bool has_negative_cycle = false;
    for (int i = 0; i < n; ++i)
    {
        if (dist[i][i] < 0)
        {
            has_negative_cycle = true;
            break;
        }
    }

    return {dist, has_negative_cycle};
}

// --- 示例 Main 函数 ---
int main()
{
    // --- 示例 1: 无权图 BFS ---
    std::cout << "--- BFS Example (Unweighted) ---" << std::endl;
    int n_bfs = 5;
    std::vector<std::vector<Edge>> adj_bfs(n_bfs);
    auto add_edge_bfs = [&](int u, int v)
    {
        adj_bfs[u].push_back({v, 1}); // 权重不重要，设为1
        adj_bfs[v].push_back({u, 1}); // 无向图
    };
    add_edge_bfs(0, 1);
    add_edge_bfs(0, 2);
    add_edge_bfs(1, 3);
    add_edge_bfs(2, 4);
    add_edge_bfs(3, 4);
    std::vector<int> dist_bfs = bfs(adj_bfs, n_bfs, 0);
    std::cout << "BFS distances from node 0:" << std::endl;
    for (int i = 0; i < n_bfs; ++i)
    {
        std::cout << "Node " << i << ": " << (dist_bfs[i] == INF_INT ? "INF" : std::to_string(dist_bfs[i])) << std::endl;
    }
    std::cout << std::endl;

    // --- 示例 2: 非负权图 Dijkstra ---
    std::cout << "--- Dijkstra Example (Non-negative weights) ---" << std::endl;
    int n_dijkstra = 5;
    std::vector<std::vector<Edge>> adj_dijkstra(n_dijkstra);
    auto add_edge_dijkstra = [&](int u, int v, int w)
    {
        adj_dijkstra[u].push_back({v, w});
        // adj_dijkstra[v].push_back({u, w}); // 如果是无向图
    };
    add_edge_dijkstra(0, 1, 10);
    add_edge_dijkstra(0, 3, 5);
    add_edge_dijkstra(1, 2, 1);
    add_edge_dijkstra(1, 3, 2);
    add_edge_dijkstra(2, 4, 4);
    add_edge_dijkstra(3, 1, 3);
    add_edge_dijkstra(3, 2, 9);
    add_edge_dijkstra(3, 4, 2);
    add_edge_dijkstra(4, 0, 7);
    add_edge_dijkstra(4, 2, 6);

    std::vector<long long> dist_dijkstra = dijkstra(adj_dijkstra, n_dijkstra, 0);
    std::cout << "Dijkstra distances from node 0:" << std::endl;
    for (int i = 0; i < n_dijkstra; ++i)
    {
        std::cout << "Node " << i << ": " << (dist_dijkstra[i] == INF_LL ? "INF" : std::to_string(dist_dijkstra[i])) << std::endl;
    }
    std::cout << std::endl;

    // --- 示例 3: 带负权图 Bellman-Ford / SPFA ---
    std::cout << "--- Bellman-Ford / SPFA Example (Negative weights) ---" << std::endl;
    int n_bf_spfa = 4;
    std::vector<EdgeInfo> edges_bf;
    std::vector<std::vector<Edge>> adj_spfa(n_bf_spfa);
    auto add_edge_bf_spfa = [&](int u, int v, int w)
    {
        edges_bf.push_back({u, v, w});
        adj_spfa[u].push_back({v, w});
        // 如果是无向图，需要添加反向边
    };
    add_edge_bf_spfa(0, 1, 5);
    add_edge_bf_spfa(0, 2, 4);
    add_edge_bf_spfa(1, 3, 3);
    add_edge_bf_spfa(2, 1, -6); // 负权边
    add_edge_bf_spfa(3, 2, 2);

    // Bellman-Ford
    auto [dist_bf, neg_cycle_bf] = bellman_ford(edges_bf, n_bf_spfa, 0);
    std::cout << "Bellman-Ford distances from node 0:" << std::endl;
    if (neg_cycle_bf)
    {
        std::cout << "Negative cycle detected!" << std::endl;
    }
    else
    {
        for (int i = 0; i < n_bf_spfa; ++i)
        {
            std::cout << "Node " << i << ": " << (dist_bf[i] == INF_LL ? "INF" : std::to_string(dist_bf[i])) << std::endl;
        }
    }

    // SPFA
    auto [dist_spfa, neg_cycle_spfa] = spfa(adj_spfa, n_bf_spfa, 0);
    std::cout << "\nSPFA distances from node 0:" << std::endl;
    if (neg_cycle_spfa)
    {
        std::cout << "Negative cycle detected!" << std::endl;
    }
    else
    {
        for (int i = 0; i < n_bf_spfa; ++i)
        {
            std::cout << "Node " << i << ": " << (dist_spfa[i] == INF_LL ? "INF" : std::to_string(dist_spfa[i])) << std::endl;
        }
    }
    std::cout << std::endl;

    // --- 示例 4: 负环检测 ---
    std::cout << "--- Negative Cycle Detection Example ---" << std::endl;
    int n_neg_cycle = 3;
    std::vector<EdgeInfo> edges_neg_cycle;
    std::vector<std::vector<Edge>> adj_neg_cycle(n_neg_cycle);
    auto add_edge_neg = [&](int u, int v, int w)
    {
        edges_neg_cycle.push_back({u, v, w});
        adj_neg_cycle[u].push_back({v, w});
    };
    add_edge_neg(0, 1, 1);
    add_edge_neg(1, 2, -1);
    add_edge_neg(2, 1, -1); // 1 -> 2 -> 1 形成负环

    auto [dist_bf_neg, neg_cycle_bf_neg] = bellman_ford(edges_neg_cycle, n_neg_cycle, 0);
    std::cout << "Bellman-Ford negative cycle detection: " << (neg_cycle_bf_neg ? "Detected" : "Not Detected") << std::endl;

    auto [dist_spfa_neg, neg_cycle_spfa_neg] = spfa(adj_neg_cycle, n_neg_cycle, 0);
    std::cout << "SPFA negative cycle detection: " << (neg_cycle_spfa_neg ? "Detected" : "Not Detected") << std::endl;
    std::cout << std::endl;

    // --- 示例 5: 所有顶点对 Floyd-Warshall ---
    std::cout << "--- Floyd-Warshall Example (All Pairs) ---" << std::endl;
    int n_fw = 4;
    std::vector<std::vector<int>> dist_fw(n_fw, std::vector<int>(n_fw, INF_INT));
    for (int i = 0; i < n_fw; ++i)
        dist_fw[i][i] = 0;

    auto add_edge_fw = [&](int u, int v, int w)
    {
        dist_fw[u][v] = w;
        // dist_fw[v][u] = w; // 如果是无向图
    };
    add_edge_fw(0, 1, 3);
    add_edge_fw(0, 3, 7);
    add_edge_fw(1, 0, 8);
    add_edge_fw(1, 2, 2);
    add_edge_fw(2, 0, 5);
    add_edge_fw(2, 3, 1);
    add_edge_fw(3, 0, 2);
    // add_edge_fw(1, 3, -5); // 可以加入负权边

    auto [shortest_paths_fw, neg_cycle_fw] = floyd_warshall(dist_fw, n_fw);

    std::cout << "Floyd-Warshall shortest distances between all pairs:" << std::endl;
    if (neg_cycle_fw)
    {
        std::cout << "Negative cycle detected!" << std::endl;
    }
    else
    {
        std::cout << "   ";
        for (int j = 0; j < n_fw; ++j)
            printf("%5d", j);
        std::cout << "\n------------------------\n";
        for (int i = 0; i < n_fw; ++i)
        {
            printf("%2d|", i);
            for (int j = 0; j < n_fw; ++j)
            {
                if (shortest_paths_fw[i][j] == INF_INT)
                    printf("%5s", "INF");
                else
                    printf("%5d", shortest_paths_fw[i][j]);
            }
            std::cout << std::endl;
        }
    }

    return 0;
}