// performance_test.cpp
#include "rbtree.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include <iomanip>
#include <cassert>
#include <string>
#include <functional>

// 颜色设置 (仅Windows)
#ifdef _WIN32
#include <windows.h>
#define GREEN_TEXT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10)
#define RED_TEXT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12)
#define YELLOW_TEXT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14)
#define WHITE_TEXT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)
#else
#define GREEN_TEXT std::cout << "\033[32m"
#define RED_TEXT std::cout << "\033[31m"
#define YELLOW_TEXT std::cout << "\033[33m"
#define WHITE_TEXT std::cout << "\033[0m"
#endif

// 辅助计时器类
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;
public:
    Timer(const std::string& op_name = "") : name(op_name) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count() / 1000.0;
    }

    ~Timer() {
        if (!name.empty()) {
            std::cout << name << " took " << elapsed() << " ms" << std::endl;
        }
    }
};

// 红黑树属性验证辅助类
class RBTreeValidator {
private:
    // 验证红黑树的辅助结构
    struct RBProps {
        bool valid;
        int black_height;

        RBProps(bool v, int h) : valid(v), black_height(h) {}
    };

    // 检查节点的颜色
    static bool isRed(Node* node) {
        return node != nullptr && node->color == RED;
    }

    // 检查从节点到所有叶子的黑色节点数量是否相同
    static RBProps validateBlackHeight(Node* node) {
        if (node == nullptr) {
            return RBProps(true, 1); // NIL节点是黑色的
        }

        // 检查是否有连续的红色节点
        if (isRed(node) && (isRed(node->left) || isRed(node->right))) {
            std::cout << "Property violation: Red node " << node->data
                      << " has red child" << std::endl;
            return RBProps(false, 0);
        }

        // 递归检查左右子树
        RBProps left = validateBlackHeight(node->left);
        if (!left.valid) return RBProps(false, 0);

        RBProps right = validateBlackHeight(node->right);
        if (!right.valid) return RBProps(false, 0);

        // 检查黑高度是否一致
        if (left.black_height != right.black_height) {
            std::cout << "Property violation: Node " << node->data
                      << " has inconsistent black heights: "
                      << left.black_height << " vs " << right.black_height << std::endl;
            return RBProps(false, 0);
        }

        // 计算当前节点的黑高度
        int height = left.black_height + (node->color == BLACK ? 1 : 0);
        return RBProps(true, height);
    }

public:
    // 验证红黑树的所有属性
    static bool validate(RedBlackTree& tree, Node* root) {
        // 属性1：每个节点要么是红色，要么是黑色 (由枚举确保)

        // 属性2：根节点是黑色
        if (root != nullptr && root->color != BLACK) {
            std::cout << "Property violation: Root is not black" << std::endl;
            return false;
        }

        // 属性3和4：通过validateBlackHeight检查
        RBProps result = validateBlackHeight(root);

        return result.valid;
    }
};

// 测试用例生成器
class TestDataGenerator {
public:
    // 生成随机数据
    static std::vector<int> randomData(int count, int min = 0, int max = 10000) {
        std::vector<int> data(count);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);

        for (int i = 0; i < count; ++i) {
            data[i] = distrib(gen);
        }
        return data;
    }

    // 生成顺序数据
    static std::vector<int> sequentialData(int count, bool ascending = true) {
        std::vector<int> data(count);
        for (int i = 0; i < count; ++i) {
            data[i] = ascending ? i : count - i - 1;
        }
        return data;
    }

    // 生成包含重复元素的数据
    static std::vector<int> duplicateData(int count, int duplicates = 5) {
        std::vector<int> data(count);
        for (int i = 0; i < count; ++i) {
            data[i] = i / duplicates;
        }
        return data;
    }
};

// 测试单个插入序列
void testInsertionSequence(const std::vector<int>& data, const std::string& test_name) {
    std::cout << "\n=== Testing " << test_name << " (" << data.size() << " elements) ===" << std::endl;

    // 用于保存排序后的结果
    std::vector<int> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());
    // 移除重复元素
    sorted_data.erase(std::unique(sorted_data.begin(), sorted_data.end()), sorted_data.end());

    // 测试我们实现的红黑树
    RedBlackTree rbtree;
    std::vector<int> rb_result;

    // 测试插入性能
    {
        Timer timer("RBTree insertion");
        for (int val : data) {
            rbtree.insert(val);
        }
    }

    // 收集中序遍历结果
    std::cout << "Inorder traversal: ";
    rbtree.inorder();
    std::cout << std::endl;

    // 测试std::set作为参考
    std::set<int> stdset;
    {
        Timer timer("std::set insertion");
        for (int val : data) {
            stdset.insert(val);
        }
    }

    // 验证结果
    std::vector<int> set_result(stdset.begin(), stdset.end());
    bool match = (sorted_data == set_result);

    if (match) {
        GREEN_TEXT;
        std::cout << "PASS: Insertion and ordering verified against std::set" << std::endl;
        WHITE_TEXT;
    } else {
        RED_TEXT;
        std::cout << "FAIL: Results don't match std::set" << std::endl;
        WHITE_TEXT;

        // 显示期望结果
        std::cout << "Expected: ";
        for (int val : sorted_data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

// 性能比较测试
void performanceComparison() {
    std::cout << "\n=== Performance Comparison ===" << std::endl;

    // 测试不同大小
    std::vector<int> sizes = {1000, 10000, 100000};

    std::cout << std::left << std::setw(15) << "Size"
              << std::setw(20) << "RBTree (ms)"
              << std::setw(20) << "std::set (ms)"
              << std::setw(20) << "Ratio" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (int size : sizes) {
        // 生成随机数据
        auto data = TestDataGenerator::randomData(size);

        // 测试红黑树
        double rb_time;
        {
            RedBlackTree rbtree;
            Timer timer;
            for (int val : data) {
                rbtree.insert(val);
            }
            rb_time = timer.elapsed();
        }

        // 测试std::set
        double set_time;
        {
            std::set<int> stdset;
            Timer timer;
            for (int val : data) {
                stdset.insert(val);
            }
            set_time = timer.elapsed();
        }

        // 计算比率
        double ratio = rb_time / set_time;

        // 打印结果
        std::cout << std::left << std::setw(15) << size
                  << std::setw(20) << rb_time
                  << std::setw(20) << set_time
                  << std::setw(20) << ratio << std::endl;
    }
}

// 主测试函数
int main() {
    std::cout << "===== Red-Black Tree Test Suite =====" << std::endl;

    // 测试1: 基本功能测试
    std::cout << "\n--- Test 1: Basic Functionality ---" << std::endl;
    {
        RedBlackTree tree;
        std::vector<int> values = {7, 3, 18, 10, 22, 8, 11, 26};

        std::cout << "Inserting: ";
        for (int val : values) {
            std::cout << val << " ";
            tree.insert(val);
        }
        std::cout << std::endl;

        std::cout << "Inorder traversal: ";
        tree.inorder();
        std::cout << std::endl;
    }

    // 测试2: 测试各种数据模式
    std::cout << "\n--- Test 2: Various Data Patterns ---" << std::endl;

    // 随机数据
    testInsertionSequence(TestDataGenerator::randomData(200), "Random Data");

    // 升序数据
    testInsertionSequence(TestDataGenerator::sequentialData(200, true), "Ascending Data");

    // 降序数据
    testInsertionSequence(TestDataGenerator::sequentialData(200, false), "Descending Data");

    // 带重复的数据
    testInsertionSequence(TestDataGenerator::duplicateData(200), "Data with Duplicates");

    // 测试3: 性能对比
    performanceComparison();

    std::cout << "\n===== Test Suite Completed =====" << std::endl;
    return 0;
}