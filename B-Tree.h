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
    void splitChild(BTreeNode<T>* x, int i) {
        BTreeNode<T>* z = new BTreeNode(); // new node to the right
        BTreeNode<T>* y = x->children[i]; // original node that was oversize
        z->leaf = y->leaf;
        z->n = t - 1;
        for(int j = 0; j<t-1; j++) {
            z->children.push_back(y->children[j+t]);
        }
        y->n = t-1;
        for(int j = x->n+1; j >= i+1; j--) {
            x->children[j+1] = x->children[j]; // shift children of x to the left
        }
        x->children[i+1] = z;
        for(int j = x->n; j >= i; j--) {
            x->keys[j+1] = x->keys[j];
        }
        x->keys[i] = y->keys[t];
        x->n += 1;
    }
    void insert(T k) {
        BTreeNode<T>* r = root;
        if(root->n == 2*t - 1) {
            BTreeNode* s = new BTreeNode();
            root = s;
            s->leaf = false;
            s->n = 0;
            s->children.push_back(r);
            splitChild(s, 1);
            insert_nonfull(s, k);
        } else {   
            insert_nonfull(r, k);
        }
    }
    void insert_nonfull(BTreeNode* x, int k) {
        int i = x->n;
        if(x->leaf) {
            while(i >= 1 && k < x->key[i]) {
                x->key[i+1] = x->key[i]; // sift through from right to left
                i -= 1;
            }
            x->key[i+1] = k;
            x->n = x->n + 1;
        } else {
            while(i >= 1 && k < x->key[i]) {
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
    }
    std::pair<BTreeNode*, int> search(BTreeNode* node, T k) {
        BTreeNode<T>* x = node;
        int i = 1;
        while(i <= x->n && k > x->keys[i]) {
            i += 1;
        }
        if(i <= x->n && k == x->keys[i]) {
            return std::pair<BTreeNode*, int>{node, i};
        }
        return search(x->keys[i], k);
    }
    void printBTree() {
        // going in a pre-order traversal (BFS)
        std::queue<BTreeNode*> q;
        q.push(root);
        while(!q.empty()) {
            BTreeNode* curr = q.top();
            q.pop();
            curr->print();
            for(int i=0; i<curr->children.size(); i++) {
                q.push(curr->children[i]);
            }
        }
    }
private:
    BTreeNode<T>* root;
    int t;
    int h;
};