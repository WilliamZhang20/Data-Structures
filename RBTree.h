#ifndef RBTREE_H
#define RBTREE_H
#include <iostream>

typedef enum { RED, BLACK } Color;

struct RBTreeNode {
    int data;
    RBTreeNode* parent;
    RBTreeNode* left;
    RBTreeNode* right;
    Color color;

    RBTreeNode(int val) 
        : data(val), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
};

class RBTree {
private:
    RBTreeNode* root;
    RBTreeNode* sentinel;

    void leftRotate(RBTreeNode* x);
    void rightRotate(RBTreeNode* x);
    void insertFix(RBTreeNode* k);

public:
    RBTree();
    ~RBTree();

    void insert(int key);
    void remove(int key);
    RBTreeNode* search(int key) const;
    RBTreeNode* predecessor() const;
    RBTreeNode* successor() const;

    void inOrderTraversal() const;
    void preOrderTraversal() const;
    void postOrderTraversal() const;
};

#endif // RBTREE_H