#include <iostream>
#include "B-Tree.h"

int main() {
    BTree<int> tree(2);
    std::cout << "before insertions\n";
    tree.insert(2);
    tree.insert(4);
    tree.insert(1);
    std::cout << "THIRD complete\n";
    tree.printBTree();
    tree.insert(5);
    std::cout << "insertions complete\n";
    tree.printBTree();
    return 0;
}