#include <vector>

template <typename T>
class BTreeNode {
    friend class BTree;
public:
    BTreeNode() {
        n = 0;
        leaf = true;
    }
    int n;
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    bool leaf;
};