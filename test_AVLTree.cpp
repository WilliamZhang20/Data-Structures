#include <iostream>
#include "AVLTree.h"

int main() {
    AVLTree<int> tree;
    std::vector<int> vals = {5, 6, 1, 0, 32, 12, 23, 25, 90};
    for(int &val : vals) {
        tree.push(std::move(val));
        std::cout << "post push\n";
        tree.print();
    }
    return 0;
}