#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include <vector>

// 线段树节点结构体 (也可以用类)
// 这里我们直接在SegmentTree类中使用一个vector来存储树节点，
// 这样更符合线段树常见的数组实现方式，可以省略单独的Node类。
// 节点信息将直接存储在tree数组中。

// 线段树类
class SegmentTree
{
private:
    std::vector<int> tree; // 用于存储线段树节点的数组 (下标从1开始)
    std::vector<int> data; // 存储原始数据的副本 (下标从0开始)
    int n;                 // 原始数据的元素个数

    /**
     * @brief 内部构建线段树的递归函数
     * @param nodeIndex 当前节点的在tree数组中的索引
     * @param start 当前节点代表的区间的左端点 (在原始数据中的索引)
     * @param end 当前节点代表的区间的右端点 (在原始数据中的索引)
     * @details 从给定的原始数据构建线段树。每个节点存储其代表区间的和。
     *          递归地构建左右子树，然后当前节点的值等于左右子节点的值之和。
     */
    void build(int nodeIndex, int start, int end);

    /**
     * @brief 内部执行区间查询的递归函数
     * @param nodeIndex 当前节点的索引
     * @param start 当前节点代表区间的左端点
     * @param end 当前节点代表区间的右端点
     * @param queryLeft 查询区间的左端点
     * @param queryRight 查询区间的右端点
     * @return int 指定查询区间 [queryLeft, queryRight] 内元素的和
     * @details 递归地在树中查找。
     *          如果当前节点区间完全包含在查询区间内，返回当前节点的值。
     *          如果当前节点区间与查询区间没有交集，返回0。
     *          否则，递归查询左右子节点，并将结果合并（求和）。
     */
    int query(int nodeIndex, int start, int end, int queryLeft, int queryRight);

    /**
     * @brief 内部执行单点更新的递归函数
     * @param nodeIndex 当前节点的索引
     * @param start 当前节点代表区间的左端点
     * @param end 当前节点代表区间的右端点
     * @param updateIndex 需要更新的原始数据的索引
     * @param newValue 要更新成的新值
     * @details 递归地找到代表 `updateIndex` 的叶子节点并更新其值。
     *          然后回溯更新从叶子节点到根节点路径上所有祖先节点的值。
     */
    void update(int nodeIndex, int start, int end, int updateIndex, int newValue);

    // 线段树通常不需要像二叉搜索树那样显式地销毁节点，
    // 因为我们使用vector，其内存会自动管理。
    // 如果使用动态分配的Node对象，则需要类似destroyTree的方法。

public:
    /**
     * @brief SegmentTree 构造函数
     * @param inputData 用于构建线段树的原始数据vector
     * @details 初始化原始数据，计算存储树所需的数组大小，并调用build函数构建树。
     */
    explicit SegmentTree(const std::vector<int> &inputData);

    /**
     * @brief SegmentTree 析构函数
     * @details 由于使用vector，通常不需要显式操作，但保留结构完整性。
     */
    ~SegmentTree();

    /**
     * @brief 公共接口：执行区间查询
     * @param queryLeft 查询区间的左端点 (在原始数据中的索引)
     * @param queryRight 查询区间的右端点 (在原始数据中的索引)
     * @return int 区间 [queryLeft, queryRight] 内元素的和
     * @details 调用内部的递归查询函数。包含边界检查。
     */
    int query(int queryLeft, int queryRight);

    /**
     * @brief 公共接口：执行单点更新
     * @param updateIndex 需要更新的原始数据的索引
     * @param newValue 要更新成的新值
     * @details 调用内部的递归更新函数。包含边界检查。
     */
    void update(int updateIndex, int newValue);
};

#endif // SEGMENTTREE_H