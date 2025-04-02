#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
/*
AVL Tree
*/

template <typename T>
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

template <typename T>
AVLTree<T>::AVLTree() :size_(0), root_(nullptr) {}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& tree) :size_(tree.size_) {
    if(tree.root_) {
        root_ = new Node(*tree.root_);
    } else {
        root_ = nullptr;
    }
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& tree) noexcept : size_(tree.size_), root_(std::exchange(tree.root_, nullptr)) { } 
// std::exchange replaces old tree root with nullptr - returns rvalue ref to the root_ ptr

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree<T>& rhs) noexcept {
    swap(*this, rhs); // dunk out the rhs - which won't be used
    return *this;
}

template <typename T>
T* AVLTree<T>::min() {
    return root_ ? root_->min() : nullptr;
}

template <typename T>
T* AVLTree<T>::max() {
    return (root_ ? root_->max() : nullptr);
}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete this->root_;
}

template <typename T>
void AVLTree<T>::push(T&& key) {
    Node::push(&root_, std::forward<T>(key));
}

template <typename T>
void AVLTree<T>::erase(const T& key) {
    Node::erase(root_, key);
}

template <typename T>
void AVLTree<T>::print() const {
    if(!this->root_) {
        std::cout << "Tree is empty\n";
    }
    std::cout << Node::print(root_);
}

/*
AVL Tree Node
*/

template <typename T>
class AVLTree<T>::Node {
    T key;
    Node* left;
    Node* right;
    Node* parent;
    short height_;
    short balance_factor;
public:
    Node(const T& key, Node* parent=nullptr); // default value of parent is nullptr (e.g. for root)
    Node(const T&& key, Node* parent=nullptr); // rvalue ref constructor
    ~Node();
    Node(const Node& node); // copy constructor
    Node(Node&& node) noexcept; // move constrcutor (optimized with noexcept)

    Node& operator=(const Node& node) = delete; // protect against copy assignment
    Node&& operator=(Node&& node) = delete; // protect against move assignment

    T* Min();
    T* Max();
    
    static std::string print(Node* curr);
    static T* Find(Node* curr, const T& key);
    static const T* Find(const Node* curr, const T& key);
    int depth();
    Node& predecessor();
    Node& successor();

    // static methods for push/erase - no overhead for objects
    // Hence, they will run faster!
    static void push(Node** curr, T&& key, Node* parent=nullptr);
    static void erase(Node* curr, const T& key);
    static void balanceSubtree(Node** node);
private:
    void updateParams();
    void RotateLeft(Node* x);
    void RotateRight(Node* x);
};

template <typename T>
AVLTree<T>::Node::Node(const T& key, Node* parent) : 
    key(key), height_(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

template <typename T>
AVLTree<T>::Node::Node(const T&& key, Node* parent) : // rvalue reference overloaded constructor
    key(std::move(key)), height_(0), balance_factor(0), left(nullptr), right(nullptr), parent(parent) {}

template <typename T>
AVLTree<T>::Node::Node(const Node& rhs) {
    left = rhs.left ? new Node(*rhs.left) : nullptr;
    right = rhs.right? new Node(*rhs.right) : nullptr;
    parent = rhs.parent; // will point to same address instead
    key = rhs.key;
    height_ = rhs.height_;
    balance_factor = rhs.balance_factor;
}

template <typename T>
AVLTree<T>::Node::Node(Node&& rhs) noexcept : height_(rhs.height_), balance_factor(rhs.balance_factor), left(std::exchange(rhs.left, nullptr)), right(std::exchange(rhs.right, nullptr)), parent(std::exchange(rhs.parent, nullptr)) { } // move constructor

template <typename T>
AVLTree<T>::Node::~Node() {
    delete left;
    delete right;
}

template <typename T>
void AVLTree<T>::Node::push(Node** curr, T&& key, Node* parent) {
    if(*curr == nullptr) {
        *curr = new Node(std::forward<T>(key), parent);
        return;
    }
    if(key < (*curr)->key) {
        push(&(*curr)->left, std::forward<T>(key), *curr);
    } else if(key > (*curr)->key) {
        push(&(*curr)->right, std::forward<T>(key), *curr);
    } else {
        return; // prohibit identical elements
    }
    // balance is called at multiple levels, lowest to highest (or deepest first)
    balanceSubtree(curr);
}

template <typename T>
void AVLTree<T>::Node::balanceSubtree(Node** node) {
    (*node)->updateParams();
    int balance = (*node)->balance_factor;

    if(balance > 1) { // left-heavy
        if((*node)->left->balance_factor >= 0) { // left-left balance
            (*node)->RotateRight(*node);
        } 
        else {
            (*node)->RotateLeft((*node)->left); // left-right balance
            (*node)->RotateRight(*node);
        }
    } 
    else if(balance < -1) { // Right-heavy
        if ((*node)->right->balance_factor <= 0) {
            (*node)->RotateLeft(*node);
        }
        // Right-Left case: Perform a right rotation followed by a left rotation
        else {
            (*node)->RotateRight((*node)->right);
            (*node)->RotateLeft(*node);
        }
    }
}

template <typename T>
void AVLTree<T>::Node::updateParams() {
    // update height_ + balance factor
    int left_height = (left != nullptr ? this->left->height_ : -1);
    int right_height = (right != nullptr ? this->right->height_ : -1);
    height_ = std::max(left_height, right_height) + 1;
    balance_factor = left_height - right_height;
}

template <typename T>
std::string AVLTree<T>::Node::print(Node* curr) { // can print tree or subtree!
    std::string tree_str = "";
    std::queue<std::pair<Node*, int>> q;
    q.push({curr, 0});
    int lvl = 0;
    tree_str += "\n";
    while (!q.empty()) {
        std::pair<Node*, int> top = q.front();
        q.pop();
        if(top.second > lvl) {
            tree_str += "\n";
            lvl += 1;
        }
        tree_str += std::to_string(top.first->key) + " ";
        if (top.first->left) q.push({top.first->left, top.second+1});
        if (top.first->right) q.push({top.first->right, top.second+1});
    }
    tree_str += "\n";
    return tree_str;
}

template <typename T>
void AVLTree<T>::Node::RotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent != nullptr) {
        if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->right = x;
    x->parent = y;
    updateParams();
    y->updateParams();
}

template <typename T>
void AVLTree<T>::Node::RotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent != nullptr) {
        if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
    updateParams();
    y->updateParams();
}