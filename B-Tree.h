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
            s->children.push_back(r);
            splitChild(s, 0);
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
        std::queue<std::pair<BTreeNode<T>*, int>> q;
        int lvl = 0;
        q.push({root, 0});
        while(!q.empty()) {
            std::pair<BTreeNode<T>*, int> curr = q.front();
            if(curr.second > lvl) {
                std::cout << "\n";
                lvl = curr.second;
            }
            q.pop();
            curr.first->printKeys();
            for(int i=0; i<curr.first->children.size(); i++) {
                q.push({curr.first->children[i], curr.second+1});
            }
        }
        std::cout << "\n";
    }
private:
    BTreeNode<T>* root;
    int t;
    int h;
    
    void splitChild(BTreeNode<T>* x, int i) {
        BTreeNode<T>* z = new BTreeNode<T>(); // New node to the right
        BTreeNode<T>* y = x->children[i];    // Original node that was oversized
        z->leaf = y->leaf;
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++) {
            z->keys.push_back(y->keys[j + t]);
        }
        if (!y->leaf) {
            for (int j = 0; j < t; j++) {
                z->children.push_back(y->children[j + t]);
            }
        }
        y->n = t - 1;

        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, y->keys[t - 1]);
        x->n += 1;
    }
    
    void insert_nonfull(BTreeNode<T>* x, int k) {
        int i = x->n - 1;
        if(x->leaf) {
            while(i >= 0 && k < x->keys[i]) {
                i -= 1;
            }
            x->keys.insert(x->keys.begin()+i+1, k);
            x->n = x->n + 1;
        } else { // recurse on insertion
            while (i >= 0 && k < x->keys[i]) {
                i--;
            }
            i++;
            if (x->children[i]->n == 2 * t - 1) {
                splitChild(x, i);
                if (k > x->keys[i]) {
                    i++;
                }
            }
            insert_nonfull(x->children[i], k);
        }
        return;
    }
};