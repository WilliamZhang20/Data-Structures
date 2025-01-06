#include <iostream>
#include "B-Tree.h"

int main() {
    BTree<char> tree(2);
    tree.insert('Q');
    tree.insert('V');
    tree.insert('Z');
    tree.insert('L');
    tree.insert('N');
    tree.insert('S');
    tree.insert('E');
    tree.insert('P');
    tree.insert('O');
    tree.insert('M');
    tree.insert('F');
    tree.printBTree();
    return 0;
}