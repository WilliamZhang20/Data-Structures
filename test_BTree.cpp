#include <iostream>
#include "B-Tree.h"
#include <stdexcept>

template<typename T>
void printTreeFormat(BTree<T> tree) {
    // print a given tree + add message for extra space!
    tree.printBTree();
    std::cout << "next seq\n";
}

template<typename T>
void deleteTreeVal(BTree<T>& tree, T val) {
    std::cout << "searching for " << val << "\n";
    std::pair<BTreeNode<T>*, int> pos = tree.search(val);
    if (pos.first) {
        std::cout << "Using found value of " << (*pos.first)[pos.second] << "\n";
        try {
            tree.remove(val);
            printTreeFormat(tree);
        } catch(const std::exception& e) {
            std::cout << e.what() << "\n";
        }
    } else {
        std::cout << "Did not find " << val << "\n";
    }
}

template<typename T, typename... Args>
void deleteTreeVals(BTree<T>& tree, Args... vals) {
    (deleteTreeVal(tree, vals), ...); // Fold expression to call deleteTreeVal for each argument
}

BTree<char> testCase1() {
    BTree<char> tree(2);
    tree.insert('Q');
    tree.insert('V');
    tree.insert('Z');
    //printTreeFormat(tree);
    tree.insert('L');
    tree.insert('N');
    tree.insert('S');
    //printTreeFormat(tree);
    tree.insert('E');
    tree.insert('P');
    tree.insert('O');
    tree.printBTree();
    //printTreeFormat(tree);
    tree.insert('M');
    tree.insert('F');
    return tree;
}

BTree<char> testCase2() {
    BTree<char> tree(2);
    // Source: CLRS 4th Ed. pg. 511
    std::vector<char> seq = {'F', 'S', 'Q', 'K', 'C', 'L', 'H', 'T', 'V', 'W', 'M', 'R', 'N', 'P', 'A', 'B', 'X', 'Y', 'D', 'Z', 'E'};
    for(int i=0; i<seq.size(); i++) {
        tree.insert(seq[i]);
        if(i % 3 == 0) {
            //printTreeFormat(tree);
        }
    }
    return tree;
}

BTree<char> testCase3() {
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
    
    printTreeFormat(tree);
    tree.remove('V');
    printTreeFormat(tree);
    
    tree.remove('N');
    printTreeFormat(tree);

    tree.remove('Z');
    printTreeFormat(tree);
    
    tree.remove('O');
    printTreeFormat(tree);
    
    tree.remove('L');
    printTreeFormat(tree);
    tree.remove('E');
    printTreeFormat(tree);
    return tree;
}

int main() {
    std::cout << "NOTE: this follows pre-emptive merge + split once at max size\n";
    std::cout << "first tree\n";
    BTree<char> tree = testCase1();
    tree.printBTree();

    std::cout << "second tree\n";

    BTree<char> tree2 = testCase2();
    tree2.printBTree();

    deleteTreeVals(tree2, 'P', 'Y', 'M', 'W', 'Q', 'Z');

    std::cout << "3rd tree\n";
    BTree<char> tree3 = testCase3();

    return 0;
}