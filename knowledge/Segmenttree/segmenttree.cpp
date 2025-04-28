#include "segmenttree.h"
#include <iostream>
#include <stdexcept> // 用于抛出异常
#include <cmath>     // 用于ceil

// 构造函数
SegmentTree::SegmentTree(const std::vector<int> &inputData)
{
    if (inputData.empty())
    {
        n = 0;
        return; // 处理空输入
    }
    n = inputData.size();
    data = inputData; // 复制原始数据

    // 计算存储线段树所需的数组大小
    // 对于一个大小为n的数组，线段树最多需要约4n的空间
    // 使用 ceil(log2(n)) 计算树的高度 h，节点数最多为 2^(h+1) - 1 < 4n
    int treeSize = 4 * n; // 分配一个保守的大小
    tree.resize(treeSize);

    // 从根节点（索引1）开始构建树，覆盖原始数据范围 [0, n-1]
    build(1, 0, n - 1);
}

// 析构函数 (由于使用vector，通常为空)
SegmentTree::~SegmentTree()
{
    // vector 会自动管理内存，无需手动释放
}

// 内部构建函数
void SegmentTree::build(int nodeIndex, int start, int end)
{
    // 到达叶子节点，叶子节点存储原始数组单个元素的值
    if (start == end)
    {
        tree[nodeIndex] = data[start];
        return;
    }

    // 递归构建左右子树
    int mid = start + (end - start) / 2; // 计算中间点，防止溢出
    int leftChildIndex = 2 * nodeIndex;
    int rightChildIndex = 2 * nodeIndex + 1;

    build(leftChildIndex, start, mid);    // 构建左子树，区间 [start, mid]
    build(rightChildIndex, mid + 1, end); // 构建右子树，区间 [mid + 1, end]

    // 当前节点的值等于左右子节点的值之和
    tree[nodeIndex] = tree[leftChildIndex] + tree[rightChildIndex];
}

// 内部查询函数
int SegmentTree::query(int nodeIndex, int start, int end, int queryLeft, int queryRight)
{
    // 情况1：当前节点代表的区间与查询区间完全没有交集
    if (end < queryLeft || start > queryRight)
    {
        return 0; // 对于求和查询，无交集贡献为0
    }

    // 情况2：当前节点代表的区间完全包含在查询区间内
    if (queryLeft <= start && end <= queryRight)
    {
        return tree[nodeIndex]; // 直接返回当前节点存储的值
    }

    // 情况3：当前节点代表的区间与查询区间部分重叠
    // 需要递归查询左右子节点，并将结果合并
    int mid = start + (end - start) / 2;
    int leftChildIndex = 2 * nodeIndex;
    int rightChildIndex = 2 * nodeIndex + 1;

    int leftSum = query(leftChildIndex, start, mid, queryLeft, queryRight);
    int rightSum = query(rightChildIndex, mid + 1, end, queryLeft, queryRight);

    return leftSum + rightSum; // 合并结果
}

// 内部更新函数
void SegmentTree::update(int nodeIndex, int start, int end, int updateIndex, int newValue)
{
    // 到达代表要更新的元素的叶子节点
    if (start == end)
    {
        // 确保我们找到了正确的叶子节点 (虽然理论上递归路径保证了这一点)
        if (start == updateIndex)
        {
            data[updateIndex] = newValue; // 更新原始数据副本 (可选，但有时有用)
            tree[nodeIndex] = newValue;   // 更新叶子节点的值
        }
        return;
    }

    // 递归地在包含 updateIndex 的子树中查找
    int mid = start + (end - start) / 2;
    int leftChildIndex = 2 * nodeIndex;
    int rightChildIndex = 2 * nodeIndex + 1;

    if (updateIndex <= mid)
    {
        // updateIndex 在左子树的区间内
        update(leftChildIndex, start, mid, updateIndex, newValue);
    }
    else
    {
        // updateIndex 在右子树的区间内
        update(rightChildIndex, mid + 1, end, updateIndex, newValue);
    }

    // 更新完子节点后，回溯更新当前节点的值
    // 当前节点的值等于其更新后的左右子节点的值之和
    tree[nodeIndex] = tree[leftChildIndex] + tree[rightChildIndex];
}

// 公共查询接口
int SegmentTree::query(int queryLeft, int queryRight)
{
    // 边界检查
    if (queryLeft < 0 || queryRight >= n || queryLeft > queryRight)
    {
        throw std::out_of_range("查询区间无效");
    }
    if (n == 0)
        return 0; // 处理空树情况
    // 从根节点（索引1）开始查询，覆盖整个原始数据范围 [0, n-1]
    return query(1, 0, n - 1, queryLeft, queryRight);
}

// 公共更新接口
void SegmentTree::update(int updateIndex, int newValue)
{
    // 边界检查
    if (updateIndex < 0 || updateIndex >= n)
    {
        throw std::out_of_range("更新索引无效");
    }
    if (n == 0)
        return; // 处理空树情况
    // 从根节点（索引1）开始更新，覆盖整个原始数据范围 [0, n-1]
    update(1, 0, n - 1, updateIndex, newValue);
}

// 示例 main 函数
int main()
{
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};
    SegmentTree st(arr);

    std::cout << "原始数组: ";
    for (int x : arr)
        std::cout << x << " ";
    std::cout << std::endl;

    // 查询区间 [1, 3] 的和 (对应元素 3, 5, 7)
    int sum1 = st.query(1, 3);
    std::cout << "区间 [1, 3] 的和: " << sum1 << std::endl; // 应为 3 + 5 + 7 = 15

    // 查询区间 [0, 5] 的和 (整个数组)
    int sum_all = st.query(0, 5);
    std::cout << "区间 [0, 5] 的和: " << sum_all << std::endl; // 应为 1+3+5+7+9+11 = 36

    // 更新索引为 2 的元素 (值为5) 为 6
    st.update(2, 6);
    std::cout << "更新索引 2 的值为 6" << std::endl;

    // 再次查询区间 [1, 3] 的和 (对应元素 3, 6, 7)
    int sum2 = st.query(1, 3);
    std::cout << "更新后区间 [1, 3] 的和: " << sum2 << std::endl; // 应为 3 + 6 + 7 = 16

    // 再次查询区间 [0, 5] 的和
    int sum_all_updated = st.query(0, 5);
    std::cout << "更新后区间 [0, 5] 的和: " << sum_all_updated << std::endl; // 应为 1+3+6+7+9+11 = 37

    // 查询单个元素 (区间 [4, 4])
    int single_element = st.query(4, 4);
    std::cout << "索引 4 的元素值: " << single_element << std::endl; // 应为 9

    return 0;
}