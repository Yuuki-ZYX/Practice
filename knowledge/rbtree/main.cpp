#include "rbtree.h"
#include <iostream>

int main()
{
    RedBlackTree tree;

    // 插入测试
    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);

    std::cout << "Inorder traversal before deletion: ";
    tree.inorder();

    // 删除测试
    tree.remove(18);
    std::cout << "Inorder traversal after deleting 18: ";
    tree.inorder();

    tree.remove(11);
    std::cout << "Inorder traversal after deleting 11: ";
    tree.inorder();

    tree.remove(7);
    std::cout << "Inorder traversal after deleting 7 (root): ";
    tree.inorder();

    return 0;
}