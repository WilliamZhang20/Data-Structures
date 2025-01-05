#pragma once
#include "B-Tree-Node.h"
#include <utility>
#include <queue>
#include <iostream>

template <typename T>
class BTree {
public:
    BTree(int t) {
        this->t = t;
        root = new BTreeNode<T>();
        root->leaf = true;
        root->n = 0;
    }
    void insert(T k) {
        BTreeNode<T>* r = root;
        if(root->n == 2*t - 1) {
            BTreeNode<T>* s = new BTreeNode<T>();
            root = s;
            s->leaf = false;
            s->n = 0;
            std::cout << "in full node\n";
            s->children.push_back(r);
            std::cout << "add child\n";
            splitChild(s, 1);
            std::cout << "split complete\n";
            insert_nonfull(s, k);
        } else {   
            insert_nonfull(r, k);
        }
    }
    std::pair<BTreeNode<T>*, int> search(BTreeNode<T>* node, T k) {
        BTreeNode<T>* x = node;
        int i = 1;
        while(i <= x->n && k > x->keys[i]) {
            i += 1;
        }
        if(i <= x->n && k == x->keys[i]) {
            return std::pair<BTreeNode<T>*, int>{node, i};
        }
        return search(x->keys[i], k);
    }
    void printBTree() {
        // going in a pre-order traversal (BFS)
        std::queue<BTreeNode<T>*> q;
        q.push(root);
        while(!q.empty()) {
            BTreeNode<T>* curr = q.front();
            q.pop();
            curr->printKeys();
            for(int i=0; i<curr->children.size(); i++) {
                q.push(curr->children[i]);
            }
        }
        std::cout << "\n";
    }
private:
    BTreeNode<T>* root;
    int t;
    int h;
    
    void splitChild(BTreeNode<T>* x, int i) {
        std::cout << "split called\n";
        BTreeNode<T>* z = new BTreeNode<T>(); // new node to the right
        BTreeNode<T>* y = x->children[i]; // original node that was oversize
        z->leaf = y->leaf;
        z->n = t - 1;
        std::cout << "checking y children\n";
        std::cout << "before loop\n";
        for(int j = 0; j<t-1; j++) {
            std::cout << "insert index " << j+t << "\n";
            z->keys.push_back(y->keys[j+t]);
        }
        y->n = t-1;
        std::cout << "before hole/??\n";
        for(int j = x->n+1; j >= i+1; j--) {
            x->children[j+1] = x->children[j]; // shift children of x to the left
        }
        x->children[i+1] = z;
        for(int j = x->n; j >= i; j--) {
            x->keys[j+1] = x->keys[j];
        }
        x->keys[i] = y->keys[t];
        x->n += 1;
        std::cout << "finish\n";
    }
    
    void insert_nonfull(BTreeNode<T>* x, int k) {
        std::cout << "insert nonfull called\n";
        int i = x->n - 1;
        if(x->leaf) {
            std::cout << "confirmed leaf\n";
            while(i >= 0 && k < x->keys[i]) {
                std::cout << "looping\n";
                x->keys[i+1] = x->keys[i]; // sift through from right to left
                i -= 1;
            }
            x->keys.insert(x->keys.begin()+i+1, k);
            x->n = x->n + 1;
        } else {
            while(i >= 0 && k < x->keys[i]) {
                i += 1;
                if(x->children[i]->n == 2*t - 1) {
                    splitChild(x, i);
                    if(k > x->keys[i]) {
                        i += 1;
                    }
                }
                insert_nonfull(x->children[i], k);
            }
        }
        return;
    }
};