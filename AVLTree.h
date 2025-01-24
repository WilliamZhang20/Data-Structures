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

    AVLTree& operator=(AVLTree& rhs) noexcept;

    void print() const;

    bool empty() const;
    int size() const;
    T* min();
    T* max();

    T find(const T& key);
    const T* find(const T& key) const;
    
    void push(T&& key); // support moving
    void erase(const T& key);
    void modifyKey(T&& newVal);
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
AVLTree<T>::AVLTree(AVLTree&& tree) noexcept : size_(tree.size_), root_(std::exchange(tree.root_, nullptr)) { } 
// std::exchange replaces old tree root with nullptr - returns rvalue ref to the root_ ptr

TEMPLATE
AVLTree<T>& AVLTree<T>::operator=(AVLTree<T>& rhs) noexcept {
    swap(*this, rhs); // dunk out the rhs - which won't be used
    return *this;
}

TEMPLATE
T* AVLTree<T>::min() {
    return root_ ? root_->min() : nullptr;
}

TEMPLATE
T* AVLTree<T>::max() {
    return (root_ ? root_->max() : nullptr);
}

TEMPLATE
AVLTree<T>::~AVLTree() {
    delete this->root_;
}

TEMPLATE
void AVLTree<T>::push(T&& key) {

}

TEMPLATE
void AVLTree<T>::erase(const T& key) {

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
    ~Node();
    Node(const Node& node); // copy constructor
    Node(Node&& node) noexcept; // move constrcutor (optimized with noexcept)

    Node& operator=(const Node& node) = delete; // protect against copy assignment
    Node&& operator=(Node&& node) = delete; // protect against move assignment

    T* Min();
    T* Max();
    std::string print(const int depth, int& max_depth, int& max_breadth);

    static T* Find(Node* curr, const T& key);
    static const T* Find(const Node* curr, const T& key);
    int depth();
    Node& predecessor();
    Node& successor();

    // static methods for push/erase - no overhead for objects
    static void push(T& key);
    static void erase(const T& key);
    static void balanceSubtree(Node** parent, Node* root);
private:
    void RotateLeft(Node& x);
    void RotateRight(Node& x);
};

TEMPLATE
AVLTree<T>::Node::Node(const T& key, Node* parent) : 
    key(key), height(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

TEMPLATE
AVLTree<T>::Node::Node(const T&& key, Node* parent) : // rvalue reference overloaded constructor
    key(std::move(key)), height(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

TEMPLATE
AVLTree<T>::Node::Node(const Node& rhs) {
    left = rhs.left ? new Node(*rhs.left) : nullptr;
    right = rhs.right? new Node(*rhs.right) : nullptr;
    parent = rhs.parent; // will point to same address instead
    key = rhs.key;
    height = rhs.height;
    balance_factor = rhs.balance_factor;
}

TEMPLATE
AVLTree<T>::Node::Node(Node&& rhs) noexcept : height(rhs.height): balance_factor(rhs.balance_factor), left(std::exchange(rhs.left, nullptr)), right(std::exchange(rhs.right, nullptr)), parent(std::exchange(rhs.parent, nullptr)) { } // move constructor

TEMPLATE
AVLTree<T>::Node::~Node() { // note: parent not in heap
    delete this->left;
    delete this->right;
}

TEMPLATE
void AVLTree<T>::Node::push(T& key) {
    
}

TEMPLATE
std::string AVLTree<T>::Node::print(const int depth, int& max_depth, int& max_breadth) {
    return ""; // temporary
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