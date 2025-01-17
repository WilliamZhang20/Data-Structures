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
    Node<T>* minNode(Node<T>* node);

    void insert(T key);
    Node<T>* search(T key);
    void deleteNode(T val); // delete node

    Node<T>* predecessor(Node<T>* node); // preceding node addr
    Node<T>* successor(Node<T>* node); // successor node addr

    void printInOrder(); // in order traversal
    bool validate(); // validation
private:
    Node<T>* root;

    bool validateHelper(Node<T>* u, T low, T high);
    void transplant(Node<T>* u, Node<T>* v);
    void deleteHelper(Node<T>* node);
};

template <typename T>
Node<T>* BST<T>::predecessor(Node<T>* node) {
    if (node->left != nullptr) {
        return minNode(node->left); // Predecessor is the max node in the left subtree
    }

    Node<T>* parent = node->parent;
    while (parent != nullptr && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent; // Find lowest ancestor whose left child is also ancestor
}

template <typename T>
Node<T>* BST<T>::successor(Node<T>* node) {
    if (node->right != nullptr) {
        return minNode(node->right); // Successor is the min node in the right subtree
    }

    Node<T>* parent = node->parent;
    while (parent != nullptr && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent; // Find lowest ancestor whose right child is also an ancestor
}

template <typename T>
bool BST<T>::validate() {
    return validateHelper(root, minVal(), maxVal());
}   

template <typename T>
bool BST<T>::validateHelper(Node<T>* u, T low, T high) {
    if(!u) {
        return true; // base
    }
    if(u->key < low || u->key > high) {
        return false;
    }
    return validateHelper(u->left, low, u->key) && validateHelper(u->right, u->key, high);
}

template <typename T>
void BST<T>::deleteNode(T key) {
    Node<T>* node = search(key);
    deleteHelper(node);
}

template <typename T>
void BST<T>::transplant(Node<T>* u, Node<T>* v) { // make v replace u
    if(u.parent == nullptr) {
        root = v;
    }
    else if(u == u.parent.left) {
        u.parent.left = v;
    }
    else {
        u.p.right = v;
    }
    if(v != nullptr) {
        v.parent = u.parent;
    }
}

template <typename T>
void BST<T>::deleteHelper(Node<T>* z) {
    if(z.left == nullptr) {
        transplant(z, z.right); // no left child - call successor
    }
    else if(z.right == nullptr) {
        transplant(z, z.left); // has left but no right - replace by predecessor
    }
    else { // has two children
        Node<T>* y = minNode(z.right);
        if(y.parent != z) {
            transplant(y, y.right);
            y.right = z.right;
            y.right.parent = y;
        }
        transplant(z, y);
        y.left = z.left;
        y.left.parent = y;
    }
}

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
    Node<T>* x = root;
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
Node<T>* BST<T>::minNode(Node<T>* node) {
    Node<T>* x = node;
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