#pragma once
#include <iostream>
#define TEMPLATE template <typename T> // custom macro!!!

/*
AVL Tree
*/

TEMPLATE
class AVLTree {
    class Node; // Nested class
    int size_;
    Node* root_;
public:
    AVLTree();
    AVLTree(const AVLTree& tree); // copy constructor
    AVLTree(AVLTree&& tree) noexcept;
    ~AVLTree();

    bool operator==(AVLTree& rhs);

    void print() const;

    bool empty() const;
    int size() const;
    T* min();
    T* max();
    
    void push(T key);
    T* find(T key);
    void modifyKey(Node& node, T newVal);
    void erase(Node& curr);
};

TEMPLATE
AVLTree<T>::AVLTree() :size_(0), root_(nullptr) {}

TEMPLATE
AVLTree<T>::AVLTree(const AVLTree& tree) :size_(tree.size) {
    if(tree.root_) {
        root_ = new Node(*tree.root_);
    }
}

TEMPLATE
void AVLTree<T>::print() const {
    
}

/*
AVL Tree Node
*/

TEMPLATE
class AVLTree<T>::Node {
    T key;
    Node* left;
    Node* right;
    Node* parent;
    short height;
    short balance_factor;
public:
    Node(const T& key, Node* parent=nullptr); // default value of parent is nullptr (e.g. for root)
    Node(const T&& key, Node* parent=nullptr); // rvalue ref constructor (interesting one...)
    Node(const T& key);
    ~Node();
    Node(const Node& node);
    Node(Node&& node) noexcept;

    Node& operator=(const Node& node) = delete; // protect against copy assignment
    Node&& operator=(Node&& node) = delete; // protect against move assignment

    std::string print(const int depth, int& max_depth, int& max_breadth);

    static T* Find(Node* curr, const T& key);
    static const T* Find(const Node* curr, const T& key);
    int depth();
    Node& predecessor();
    Node& successor();
private:
    void RotateLeft(Node& x);
    void RotateRight(Node& x);
};

TEMPLATE
AVLTree<T>::Node::Node(const T& key, Node* parent) : 
    key(key), height(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

TEMPLATE
AVLTree<T>::Node::Node(const T&& key, Node* parent) : 
    key(std::move(key)), height(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

TEMPLATE
std::string AVLTree<T>::Node::print(const int depth, int& max_depth, int& max_breadth) {
    
}

TEMPLATE 
void AVLTree<T>::Node::RotateRight(Node& x) { // x is bottom-most node whose subtrees are out of balance
    Node* y = x.left;
    x.left = y->right; 
    if(y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x.parent; // move left node up
    if(x == x.parent->right) {
        x.parent->left = y;
    } else {
        x.parent->left = y;
    }
    y->right = x; // x is right child of y now
    x.parent = y;
}

TEMPLATE
void AVLTree<T>::Node::RotateLeft(Node& x) {
    Node* y = x.right;
    x.right = y->left; // turn y’s left subtree into x’s right subtree
    if(y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x.parent;
    if(x == x.parent->left) {
        x.parent->left = y;
    }
    else {
        x.parent->right = y;
    }
    y->left = x;
    x.parent = y;
}