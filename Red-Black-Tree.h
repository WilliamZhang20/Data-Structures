#pragma once
#include <iostream>
#include <queue>
#include <utility>
#include <cmath>

typedef enum { RED, BLACK } Color;

template <typename T>
class RBTreeNode {
public:
    RBTreeNode<T>* parent;
    RBTreeNode<T>* left;
    RBTreeNode<T>* right;
    Color color;
    T key;
    RBTreeNode(T val, Color color_=RED) 
        : key(val), parent(nullptr), left(nullptr), right(nullptr), color(color_) {}
    
    ~RBTreeNode() {
        if(left) { delete left; }
        if(right) { delete right; }
    }
};

// Since template - should include error handling if type does not overload <  or > 
template <typename T>
class RBTree {
private:
    int size;
    RBTreeNode<T>* root;
    RBTreeNode<T>* sentinel;

    void leftRotate(RBTreeNode<T>* x);
    void rightRotate(RBTreeNode<T>* x);
    void insertFixup(RBTreeNode<T>* z); // see CLRS for why...
    void removeFixup(RBTreeNode<T>* z);

    void transplant(RBTreeNode<T>* u, RBTreeNode<T>* v); // replaces subtree of u with that of v

public: // simple API...
    RBTree();
    ~RBTree();

    void insert(T val);
    void remove(T val);
    RBTreeNode<T>* search(T val) const;

    RBTreeNode<T>* treeMin(RBTreeNode<T>* x);
    RBTreeNode<T>* treeMax(RBTreeNode<T>* x);
    void print();
};

template <typename T>
RBTree<T>::RBTree() {
    sentinel = new RBTreeNode<T>(0, BLACK);
    sentinel->parent = sentinel; // sentinel's parent is itself - for safety
    root = sentinel;
}

template <typename T>
RBTree<T>::~RBTree() {
    delete sentinel;
}

template <typename T>
RBTreeNode<T>* RBTree<T>::treeMin(RBTreeNode<T>* x) {
    if(x->left != sentinel) {
        return treeMin(x->left);
    }
    return x;
}

template <typename T>
RBTreeNode<T>* RBTree<T>::treeMax(RBTreeNode<T>* x) {
    if(x->right != sentinel) {
        return treeMin(x->right);
    }
    return x;
}

template <typename T>
void RBTree<T>::leftRotate(RBTreeNode<T>* x) { // throw X to the left - replace with its right child
    RBTreeNode<T>* y = x->right;
    x->right = y->left; // y's left subtree --> x's right subtree
    if(y->left != sentinel) { // if it wasn't empty
        y->left->parent = x; // then ensure that its parent is X
    }
    y->parent = x->parent;
    if(x->parent == sentinel) {
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
    if(x->parent == sentinel) {
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
    this->size++;
    // Create new node z
    RBTreeNode<T>* z = new RBTreeNode<T>(val, RED);
    // Insertion process below
    RBTreeNode<T>* x = root; // node compared with z
    RBTreeNode<T>* y = sentinel; // y is parent of z
    while(x != sentinel) { // descend until reaching sentinel
        y = x;
        if(z->key < x->key) {
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
    z->left = sentinel; // "insertion happens at leaf" - UW professor
    z->right = sentinel;
    this->insertFixup(z);
}

template <typename T>
void RBTree<T>::insertFixup(RBTreeNode<T>* z) {
    while(z->parent->color == RED) { // violates RB Tree property (z is RED & parent RED, z should be BLACK)
        if(z->parent == z->parent->parent->left) { // z's parent is a LEFT child
            RBTreeNode<T>* y = z->parent->parent->right; // z's right uncle 
            if(y->color == RED) { // parent & uncle are both red - CASE 1
                z->parent->color = BLACK; // both of the grandparent's children are blackened
                y->color = BLACK;
                z->parent->parent->color = RED; // redden z's grandparent
                z = z->parent->parent; // transfer the z pointer to the grandparent
            } else {
                if(z == z->parent->right) { // case 2 - z is a RIGHT Child + uncle is BLACK
                    // unbalanced tree
                    z = z->parent;
                    leftRotate(z); // then rotate over - transforms to the next case
                }
                z->parent->color = BLACK; // case 3 - uncle is black, z is LEFT child
                z->parent->parent->color = RED; // fixes black height property 
                rightRotate(z->parent->parent); // rebalances so black is root of subtree
            }
        } else { // then z's parent is a RIGHT child - SYMMETRIC to above part
            RBTreeNode<T>* y = z->parent->parent->left;
            if(y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED; 
                z = z->parent->parent; 
            } else {
                if(z == z->parent->left) { 
                    z = z->parent;
                    rightRotate(z); 
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    } // repeats only with case 1...where the property of red children is violated temporarily
    root->color = BLACK;
}

template <typename T>
void RBTree<T>::transplant(RBTreeNode<T>* u, RBTreeNode<T>* v) {
    if(u->parent == sentinel) {
        root = v;
    } else if(u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

template <typename T>
void RBTree<T>::remove(T val) {
    RBTreeNode<T>* z = search(val);
    RBTreeNode<T>* y = z;
    RBTreeNode<T>* x = nullptr;
    Color y_Orig = y->color;
    if(z->left == sentinel) {
        x = z->right;
        transplant(z, z->right);
    }
    else if(z->right == sentinel) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = treeMin(z->right);
        y_Orig = y->color;
        x = y->right;
        if(y != z->right) {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else {
            x->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_Orig == BLACK) {
        removeFixup(x);
    }
}

template <typename T>
void RBTree<T>::removeFixup(RBTreeNode<T>* z) {

}

template <typename T>
RBTreeNode<T>* RBTree<T>::search(T val) const {
    RBTreeNode<T>* x = root; // node compared with z
    while(x != sentinel) { // descend until reaching sentinel
        if(val == x->key) {
            return x;
        }
        else if(val < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return nullptr;
}

template <typename T>
void RBTree<T>::print() {
    int height = ceil(log(size + 1.0)/log(2.0));  // Calculate tree height
    std::queue<std::pair<RBTreeNode<T>*, int>> treeQ;
    treeQ.push({root, 0});
    int level = -1;

    int numSpaces = (1 << (height)) * 2; 
    
    while (!treeQ.empty()) {
        std::pair<RBTreeNode<T>*, int> curr = treeQ.front();
        treeQ.pop();
        auto printSpaces = [&]() {
            for(int i=0; i<numSpaces; i++) { std::cout << " ";}
            std::cout << " ";
            for(int i=0; i<numSpaces; i++) { std::cout << " ";}
        };
        if (curr.second > level) {
            level = curr.second;
            std::cout << "\n";
            for(int i=0; i<numSpaces; i++) { std::cout << " "; }
            numSpaces /= 2;
        }
        
        if (curr.first == sentinel) { 
            printSpaces();
            continue; 
        }
        std::cout << curr.first->key;
        std::cout << (curr.first->color == RED ? "*" : "^");
        printSpaces();
        treeQ.push({curr.first->left, curr.second + 1});
        treeQ.push({curr.first->right, curr.second + 1});
    }
    std::cout << "\n";
}
