#pragma once
#include <iostream>

typedef enum { RED, BLACK } Color;

template <typename T>
class RBTreeNode {
public:
    RBTreeNode<T>* parent;
    RBTreeNode<T>* left;
    RBTreeNode<T>* right;
    Color color_;
    T key;
    RBTreeNode(T val, Color color=RED) 
        : key(val), parent(nullptr), left(nullptr), right(nullptr), color_(color) {}
    
    ~RBTreeNode() {
        delete left;
        delete right;
    }
};

// Since template - should include error handling if type does not overload <  or > 
template <typename T>
class RBTree {
private:
    RBTreeNode<T>* root;
    RBTreeNode<T>* sentinel;

    void leftRotate(RBTreeNode<T>* x);
    void rightRotate(RBTreeNode<T>* x);
    void insertFixup(RBTreeNode<T>* z); // see CLRS for why...
    void deleteFixUp(RBTreeNode<T>* z);

    void transplant(RBTreeNode<T>* u, RBTreeNode<T>* v); // replaces subtree of u with that of v

public: // simple API...
    RBTree();
    ~RBTree();

    void insert(T val);
    void remove(T val);
    RBTreeNode<T>* search(T val) const;

    void print();
};

template <typename T>
RBTree<T>::RBTree() {
    sentinel = new RBTreeNode(0, BLACK);
    sentinel->parent = sentinel; // sentinel's parent is itself - for safety
    root = sentinel;
}

template <typename T>
RBTree<T>::~RBTree() {
    delete sentinel;
    delete root;
}

template <typename T>
void RBTree<T>::leftRotate(RBTreeNode<T>* x) { // throw X to the left - replace with its right child
    RBTreeNode<T>* y = x->right;
    x->right = y->left; // y's left subtree --> x's right subtree
    if(y->left != sentinel) { // if it wasn't empty
        y->left->parent = x; // then ensure that its parent is X
    }
    y->parent = x->parent;
    if(x->parent = sentinel) {
        root = y; // will already have a parent - the sentinel!
    } else if(x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template <typename T>
void RBTree<T>::rightRotate(RBTreeNode<T>* x) { // throw X to the right - replace with its left child
    RBTreeNode<T>* y = x->left;
    x->left = y->right; // y's left subtree --> x's right subtree
    if(y->right != sentinel) { // if it wasn't empty
        y->right->parent = x; // then ensure that its parent is X
    }
    y->parent = x->parent;
    if(x->parent = sentinel) {
        root = y;
    } else if(x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

template <typename T>
void RBTree<T>::insert(T val) {
    // Create new node z
    RBTreeNode<T>* z = new RBTreeNode<T>*(val, RED);
    // Insertion process below
    RBTreeNode<T>* x = root; // node compared with z
    RBTreeNode<T>* y = sentinel; // y is parent of z
    while(x != sentinel) { // descend until reaching sentinel
        y = x;
        if(z->key < y->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y; // found the location - insert z with parent y - decide if root, left or right
    if(y==sentinel) {
        root = z; // tree was empty
    } else if(z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = sentinel; // "insertion happens at leaf"
    z->right = sentinel;
    this->insertFixup(z);
}

template <typename T>
void RBTree<T>::insertFixup(RBTreeNode<T>* z) {
    while(z->parent->color == RED) {
        if(z->parent == z->parent->parent->left) { // z's parent is a LEFT child
            RBTreeNode<T>* y = z->parent->parent->right; // z's uncle 
            if(y->color == RED) { // parent & uncle are both red - CASE 1
                z->parent->color = BLACK; // both of the grandparent's children are blackened
                y->color = BLACK;
                z->parent->parent->color = RED; // redden z's grandparent
                z = z->parent->parent; // transfer the z pointer to the grandparent
            } else {
                if(z == z->parent->right) { // case 2 - z is a RIGHT Child
                    z = z->parent;
                    leftRotate(z); // then rotate over
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBTreeNode<T>* y = z->parent->parent->left; // z's other uncle 
            if(y->color == RED) { // parent & uncle are both red - CASE 1
                z->parent->color = BLACK; // both of the grandparent's children are blackened
                y->color = BLACK;
                z->parent->parent->color = RED; // redden z's grandparent
                z = z->parent->parent; // transfer the z pointer to the grandparent
            } else {
                if(z == z->parent->left) { // case 2 - z is a RIGHT Child
                    z = z->parent;
                    rightRotate(z); // then rotate over
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

template <typename T>
void RBTree<T>::remove(T val) {

}

template <typename T>
RBTreeNode<T>* RBTree<T>::search(T val) const {

}

template <typename T>
void RBTree<T>::print() {

}

int main() {
    std::cout << "Start of program\n";
    RBTree<int> test;

    return 0;
}