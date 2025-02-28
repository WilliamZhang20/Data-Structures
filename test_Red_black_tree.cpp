#include "Red-Black-Tree.h"
#include <string>

int main() {
    RBTree<int> tree;
    while(true) {
        std::string input;
        std::cin >> input;
        if(input == "in") {
            int val;
            std::cin >> val;
            tree.insert(val);
        }
        else if(input == "p") {
            tree.print();
        }
        else {
            break;
        }
    }
    return 0;
}