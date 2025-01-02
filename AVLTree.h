#include <iostream>

/*
AVL Tree
*/

template <typename T>
class AVLTree {
    class Node; // NESTED CLASS
    int size;
    Node* _root;
public:
    AVLTree();
    AVLTree(const AVLTree& tree); // copy constructor
    AVLTree(AVLTree&& tree) noexcept;
    ~AVLTree();

};

/*
AVL Tree Node
*/

template <typename T>
class AVLTree<T>::Node {
    T key;
    Node* left;
    Node* right;
    short height;
    short balance_factor;
public:
    
};