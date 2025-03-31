#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

// Forward declare BTree for node access
template <typename T>
class BTree;

template <typename T>
class BTreeNode {
    friend class BTree<T>;
public:
    // Rule of 5 class!
    BTreeNode() {
        n = 0;
        leaf = true;
    }
    ~BTreeNode() {
        for(size_t i=0; i<children.size(); i++) {
            delete children[i]; // will recurse
        }
    }
    BTreeNode(BTreeNode<T>& node) : n(node.n), leaf(node.leaf), keys(node.keys), children(node.children) {}
    BTreeNode(BTreeNode<T>&& node) noexcept : n(node.n), leaf(node.leaf), keys(std::move(node.keys)), children(std::move(node.children)) {
        // node - vector move defaults to std::vector move constructor     
        std::cout << "move constructor called\n";   
    }
    BTreeNode& operator=(BTreeNode<T>& rhs) { // perform deep copy
        if(this != &rhs) {
            n = rhs.n;
            leaf = rhs.leaf;
            keys = rhs.keys;
            children = rhs.children;
        }
        return *this;
    }
    BTreeNode& operator=(BTreeNode<T>&& rhs) noexcept {
        std::cout << "move assignment called\n";
        if (this != &rhs) {
            n = rhs.n;
            leaf = rhs.leaf;
            keys = std::move(rhs.keys);
            children = std::move(rhs.children);

            rhs.n = 0;
            rhs.leaf = true;
        }
        return *this;
    }
private:
    size_t n; // # of keys stored in page
    std::vector<T> keys; // the keys themselves
    std::vector<BTreeNode*> children; // pointers to children of size n+1
    bool leaf; // whether it is a leaf page
    void printKeys() {
        for(size_t i=0; i<n; i++) {
            std::cout << keys[i] << " ";
        }
        std::cout << "\t"; // end of page/node
    }
};

template <typename T>
class BTree {
public:
    BTree(size_t t) {
        this->t = t;
        root = new BTreeNode<T>();
        root->leaf = true;
        root->n = 0;
    }
    ~BTree() {
        delete root;
    }
    BTree(BTree<T> &tree) : t(tree.t), root(tree.root) {}
    BTree(BTree<T> &&tree) noexcept : t(tree.t), root(std::exchange(tree.root, nullptr)) {}
    BTree& operator=(BTree<T>& rhs) {
        if (this != &rhs) {
            delete root;
            t = rhs.t;
            root = new BTreeNode<T>(*rhs.root);
        }
        return *this;
    }
    BTree& operator=(BTree<T>&& rhs) noexcept {
        if (this != &rhs) {
            delete root;
            t = rhs.t;
            root = std::exchange(rhs.root, nullptr); // trash the rhs, make this own its addr
        }
    }
    void insert(T k) { // 2 cases - full or not full
        // calls recursive helper insert_nonfull
        BTreeNode<T>* r = root;
        if(root->n == 2*t - 1) { // preemptively split!
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
    
    // Returns pointer to node + index within node that it was found
    std::pair<BTreeNode<T>*, int> search(BTreeNode<T>* x, T k) {
        int i = 1;
        while(i <= x->n && k > x->keys[i]) {
            i += 1;
        } // proceed from left to right of page until appropriate range discovered
        if(i <= x->n && k == x->keys[i]) {
            // the node may be the destination...if so, return
            return std::pair{x, i};
        } // otherwise, continue at an additional depth
        else if(x->leaf) {
            return std::pair{nullptr, -1}; // garbage pair
        }
        // "else case"
        return search(x->keys[i], k); // reads the deeper "disk" + continue
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

    // No pseudocode found in CLRS
    void remove(T k) { // remove node with value k
        if(!root) { throw std::runtime_error("Empty tree!"); }
        // no need to handle underfull root - it is allowed to be empty
        deleteKeyHelper(root, k);
        if(root->n == 0) { // somehow potentially the root was borrowed out...
            BTreeNode<T>* oldRoot = root;
            // Replace root with either nothing (if tree emptied) or its only child!
            root = root->children.empty()? nullptr : root->children[0];
            delete oldRoot; // address redundant
        }
    }
private:
    BTreeNode<T>* root;
    size_t t; // page-characteristic value
    int h;
    
    void splitChild(BTreeNode<T>* x, int i) {
        BTreeNode<T>* z = new BTreeNode<T>(); // New node to the right
        BTreeNode<T>* y = x->children[i];    // Original node that was oversized
        z->leaf = y->leaf;
        z->n = t - 1;

        // Move the last (t - 1) keys from y to z
        for (int j = 0; j < t - 1; j++) {
            z->keys.push_back(y->keys[j + t]);
        }

        if (!y->leaf) {
            for (int j = 0; j < t; j++) {
                z->children.push_back(y->children[j + t]);
            }
            // Erase transferred children from y
            y->children.erase(y->children.begin() + t, y->children.end());
        }

        // Reduce the number of keys in y
        y->keys.erase(y->keys.begin() + t - 1, y->keys.end());
        y->n = t - 1;

        // insert child z into z's children
        x->children.insert(x->children.begin() + i + 1, z);

        // insert middle key into top page
        x->keys.insert(x->keys.begin() + i, y->keys[t - 1]);
        x->n += 1;
    }
    
    void insert_nonfull(BTreeNode<T>* x, int k) { // also recursive!
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

    // Considering moving repetetive parts of cases 2 & 3 into own (inline) methods
    void deleteKeyHelper(BTreeNode<T>* x, T k) {
        // search the tree & find destination!
        size_t i = x->n - 1;
        // Case 1: if arrive at leaf - delete key k if existing!
        if(x->leaf) { // IE: the base case
            while(i >= 0 && k < x->keys[i]) {
                i -= 1;
            }
            if(x->keys[i] == k) {
                x->keys.erase(x->keys.begin() + i);
                x->n -= 1;
            }
        }
        else {
            while(i >= 0 && k < x->keys[i]) {
                i -= 1; // search for node in k's value range
            }
            // Case 2: if arrived at an internal node with k
            if(x->keys[i] == k) {
                // 2a: if predecessor k' of good capacity, delete k' recursively & replace value on k
                BTreeNode<T>* predec = x->children[i];
                T kprime = predec->keys[predec->keys.size()-1];
                if(predec->n > t-1) {
                    deleteKeyHelper(predec, kprime);
                    x->keys[i] = kprime; // value replacement
                }
                // 2b: predec. underfull, but not succ. - do the same as a but for succ.
                else {
                    BTreeNode<T>* succ = x->children[i+1];
                    if(succ->n > t-1) {
                        deleteKeyHelper(succ, k);
                        x->keys[i] = succ->keys[0];
                    }
                    // 2c: both pred. and succ. underfull - merge predec & succ with k, free succ., rm k rec.
                    else {
                        // x will have no more k and no more x.c_i+1, x_ci should be full
                        mergeNodes(predec, succ, k);
                        x->n -= 1;
                        delete succ;
                        x->keys.erase(x->keys.begin()+i);
                        // deletion now depends on x.c_i & its children
                        deleteKeyHelper(predec, k);
                    }
                }
            }
            // Case 3: internal node, en route to search for k
            else {
                // 3a: if child of k's range underfull but sibling has capacity, borrow
                // (with view from the parent)
                size_t kIdx = 0;
                for(size_t j=0; j<x->children.size(); j++) {
                    if(x->children[j]->keys[x->children[j]->keys.size()-1] >= k) {
                        kIdx = j;
                        break;
                    }
                    else if(j == x->children.size() - 1) {
                        throw "Tree Structure Wrong";
                    }
                }
                BTreeNode<T>* child = x->children[kIdx];
                if(child->n == t-1) {
                    // try left sibling
                    if(kIdx > 0 && x->children[kIdx-1]->n >= t) {
                        BTreeNode<T>* leftSibling = x->children[kIdx - 1];
                        child->keys.insert(child->keys.begin(), x->keys[kIdx - 1]);
                        x->keys[kIdx - 1] = leftSibling->keys.back();
                        leftSibling->keys.pop_back();
                        child->n += 1;
                        leftSibling->n -= 1;
                    }
                    // try right sibling
                    else if(kIdx < x->children.size()-1 && x->children[kIdx+1]->n >= t) {
                        BTreeNode<T>* rightSibling = x->children[kIdx + 1];
                        child->keys.push_back(x->keys[kIdx]);
                        x->keys[kIdx] = rightSibling->keys.front();
                        rightSibling->keys.erase(rightSibling->keys.begin());
                        child->n += 1;
                        rightSibling->n -= 1;
                    }
                    // 3b: if all siblings underfull - merge with first available sibling
                    else {
                        if(kIdx > 0) {
                            BTreeNode<T>* leftSibling = x->children[kIdx - 1];
                            mergeNodes(leftSibling, child, x->keys[kIdx - 1]);
                            x->keys.erase(x->keys.begin() + kIdx - 1);
                            x->n -= 1;
                            delete child;
                        }
                        else {
                            BTreeNode<T>* rightSibling = x->children[kIdx + 1];
                            mergeNodes(child, rightSibling, x->keys[kIdx]);
                            x->keys.erase(x->keys.begin() + kIdx);
                            x->n -= 1;
                            delete rightSibling;
                        }
                    }
                }
                deleteKeyHelper(x->children[kIdx], k);
            }
        }   
    }

    // Merging must take place between two nodes AND a key
    // Occurs when both lhs and rhs have t-1 keys - combined 2t children
    // However, both nodes have combined 2t-2 keys - so need 1 extra key
    // When applied, lexic. order must be lhs << k << rhs 
    void mergeNodes(BTreeNode<T>* lhs, BTreeNode<T>* rhs, T k) {
        // append keys
        lhs->keys.push_back(k);
        lhs->keys.insert(lhs->keys.end(), rhs->keys.begin(), rhs->keys.end());
        // append children
        lhs->children.insert(lhs->children.end(), rhs->children.begin(), rhs->children.end());
        // increment size
        lhs->n += rhs->n + 1;
    }
};
