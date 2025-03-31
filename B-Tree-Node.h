#pragma once
#include <vector>

// Forward declare BTree for node access
template <typename T>
class BTree;

template <typename T>
class BTreeNode {
    friend class BTree<T>;
public:
    // Rule of 5 class!
    BTreeNode() {
        n = 0;
        leaf = true;
    }
    ~BTreeNode() {
        for(size_t i=0; i<children.size(); i++) {
            delete children[i]; // will recurse
        }
    }
    BTreeNode(BTreeNode<T>& node) : n(node.n), leaf(node.leaf), keys(node.keys), children(node.children) {}
    BTreeNode(BTreeNode<T>&& node) noexcept : n(node.n), leaf(node.leaf), keys(std::move(node.keys)), children(std::move(node.children)) {
        // node - vector move defaults to std::vector move constructor     
        std::cout << "move constructor called\n";   
    }
    BTreeNode& operator=(BTreeNode<T>& rhs) { // perform deep copy
        if(this != &rhs) {
            n = rhs.n;
            leaf = rhs.leaf;
            keys = rhs.keys;
            children = rhs.children;
        }
        return *this;
    }
    BTreeNode& operator=(BTreeNode<T>&& rhs) noexcept {
        std::cout << "move assignment called\n";
        if (this != &rhs) {
            n = rhs.n;
            leaf = rhs.leaf;
            keys = std::move(rhs.keys);
            children = std::move(rhs.children);

            rhs.n = 0;
            rhs.leaf = true;
        }
        return *this;
    }
private:
    size_t n; // # of keys stored in page
    std::vector<T> keys; // the keys themselves
    std::vector<BTreeNode*> children; // pointers to children of size n+1
    bool leaf; // whether it is a leaf page
    void printKeys() {
        for(size_t i=0; i<n; i++) {
            std::cout << keys[i] << " ";
        }
        std::cout << "\t"; // end of page/node
    }
};
