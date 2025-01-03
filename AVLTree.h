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

    bool operator==(AVLTree& rhs);

    bool empty() const;
    int size() const;
    T min();
    T max();
    
    void push(T key);
    T* find(T key);
    void modifyKey(Node& node, T newVal);
    void erase(Node& curr);
private:
    void RotateLeft(Node& x);
    void RotateRight(Node& x);
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
    Node& operator=(const Node& node) = delete;
    Node&& operator=(node&& node) = delete; // protect against bad operations
    int depth();
    int height();
    Node& predecessor();
    Node& successor();
};