#ifndef BST_H
#define BST_H
#include <iostream>

template <typename T>
struct Node {
    Node* left;
    Node* right;
    Node* parent;
    T key;
    Node() : left(nullptr), right(nullptr), key(0) {}
    Node(T key) : left(nullptr), right(nullptr), key(key) {}
    ~Node() { delete left; delete right; }
};

template <typename T>
class BST {
public:
    BST() : root(nullptr) {}
    BST(const BST<T>& tree) = delete; // no copy
    BST(BST<T>&& tree) = delete; // no moving
    ~BST() { delete root; }

    Node<T>* maxNode(); // max but node
    T maxVal();
    T minVal();
    Node<T>* minNode();

    void insert(T key);
    Node<T>* search(T key);
    void deleteNode(T val); // delete node

    Node<T>* predecessor(Node* node); // preceding node addr
    Node<T>* successor(Node* node); // successor node addr

    void printInOrder(); // in order traversal
    bool validate(); // validation
private:
    Node<T>* root;
};

template <typename T>
Node<T>* BST<T>::maxNode() {
    Node<T>* x = root;
    while(x != nullptr && x.right != nullptr) {
        x = x.right;
    }
    return x;
}

template <typename T>
T BST<T>::maxVal() {
    Node* x = root;
    while(x != nullptr && x.right != nullptr) {
        x = x.right;
    }
    if(x == nullptr) {
        return 0;
    } else {
        return x.key;
    }
}

template <typename T>
Node<T>* BST<T>::minNode() {
    Node<T>* x = root;
    while(x != nullptr && x.left != nullptr) {
        x = x.left;
    }
    return x;
}

template <typename T>
T BST<T>::minVal() {
    Node<T> x = root;
    while(x != nullptr && x.left != nullptr) {
        x = x.left;
    }
    if(x == nullptr) {
        return 0;
    }
    else {
        return x.key;
    }
}

template <typename T>
void BST<T>::insert(T key) { // insertion only happens at the leaf, so no left or right to z
    Node<T>* y = nullptr;
    Node<T>* x = root;
    Node<T>* z(nullptr, nullptr, key);
    while(x != nullptr) { // propagates down the tree
        y = x;
        if(z.key < x.key) {
            x = x.left;
        } else {
            x = x.right;
        }
    }
    z.parent = y;
    if(y == nullptr) {
        root = z;
    } else if(z.key < y.key) {
        y.left = z;
    } else {
        y.right = z;
    }
}

template <typename T>
Node<T>* BST<T>::search(T key) {
    // implementing iteratively 
    Node<T>* x = root;
    while(x != nullptr && key != x.key) {
        if(key < x.key) {
            x = x.left;
        } else {
            x = x.right;
        }
    }
    return x;
}

#endif // BST_H