#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm> 

// Forward declaration of the Node structure
template <typename T>
struct BinomialNode;

// BinomialHeap 
template <typename T, typename Predicate = std::greater<T>>
class BinomialHeap {
private:
    std::vector<std::unique_ptr<BinomialNode<T>>> rootList;

    std::unique_ptr<BinomialNode<T>> mergeTrees(std::unique_ptr<BinomialNode<T>> t1,
                                                std::unique_ptr<BinomialNode<T>> t2,
                                                Predicate p = Predicate());

    // Helper function to consolidate the root list 
    void consolidate();

    // Helper to find the minimum node
    BinomialNode<T>* findMinNode() const;

    void printTree(const BinomialNode<T>* node, int indent) const;

public:
    BinomialHeap() = default;

    ~BinomialHeap() = default;

    // Copy constructor and assignment are deleted to prevent incorrect copying
    BinomialHeap(const BinomialHeap& other) = delete;
    BinomialHeap& operator=(const BinomialHeap& other) = delete;

    // Move constructor and move assignment
     BinomialHeap(BinomialHeap&& other) noexcept
        : rootList(std::move(other.rootList)) {}

    BinomialHeap& operator=(BinomialHeap&& other) noexcept {
        if (this != &other) {
            rootList = std::move(other.rootList);
        }
        return *this;
    }

    void insert(const T& value);
    T findMin() const;
    T extractMin();

    void merge(BinomialHeap<T>&& rhs) noexcept;
    bool isEmpty() const { return rootList.empty(); }
    size_t size() const; 

    //clear function
    void clear();

    void printHeap() const;
};

// Binomial Node (for a single tree)....
template <typename T>
struct BinomialNode {
    T data;
    int degree;
    std::unique_ptr<BinomialNode<T>> child;
    std::unique_ptr<BinomialNode<T>> sibling;

    // Constructor for BinomialNode<T>
    BinomialNode(const T& val) : data(val), degree(0) {}
};

template <typename T, typename Predicate>
void BinomialHeap<T, Predicate>::insert(const T& value) {
    std::unique_ptr<BinomialNode<T>> newNode = std::make_unique<BinomialNode<T>>(value);
    BinomialHeap<T> tempHeap;
    tempHeap.rootList.push_back(std::move(newNode)); // Move the unique_ptr

    // Merge the temporary heap with the rest of the heap for good distribution
    merge(std::move(tempHeap));
}

template <typename T, typename Predicate>
std::unique_ptr<BinomialNode<T>> BinomialHeap<T, Predicate>::mergeTrees(
    std::unique_ptr<BinomialNode<T>> t1, std::unique_ptr<BinomialNode<T>> t2,
    Predicate p) {
    if (p(t1->data, t2->data)) {
        std::swap(t1, t2);
    }
    t2->sibling = std::move(t1->child);
    t1->child = std::move(t2);
    ++(t1->degree);
    return t1;
}

template <typename T, typename Predicate> // kind of like a move assignment
void BinomialHeap<T, Predicate>::merge(BinomialHeap<T>&& rhs) noexcept {
    // Similar to adding 2 binary numbers
    // simple case - one empty
    if (rhs.rootList.empty()) {
        return; 
    }
    if (this->rootList.empty()) {
        this->rootList = std::move(rhs.rootList);
        return;
    }

    std::vector<std::unique_ptr<BinomialNode<T>>> mergedList;
    size_t i = 0, j = 0;
    while(i < this->rootList.size() && j < rhs.rootList.size()) {
        if(this->rootList[i]->degree <= rhs.rootList[j]->degree) {
            mergedList.push_back(std::move(this->rootList[i++]));
        } else {
            mergedList.push_back(std::move(rhs.rootList[j++]));
        }
    }

    while(i < this->rootList.size()) {
        mergedList.push_back(std::move(this->rootList[i++]));
    }
    while(j < rhs.rootList.size()) {
        mergedList.push_back(std::move(rhs.rootList[j++]));
    }

    // move over to main root list for further processing
    this->rootList = std::move(mergedList);

    // Below: analog of collecting carried values in addition
    consolidate();
}

template <typename T, typename Predicate>
void BinomialHeap<T, Predicate>::consolidate() {
    if(this->rootList.size() <= 1) return;

    std::vector<std::unique_ptr<BinomialNode<T>>> temp;
    size_t n = rootList.size();
    temp.resize(n+1);

    for(size_t i = 0; i < n; ++i) {
        std::unique_ptr<BinomialNode<T>> curr = std::move(rootList[i]);
        int d = curr->degree;
        while(temp[d] != nullptr) { // double tree - need merging
            curr = mergeTrees(std::move(curr), std::move(temp[d]));
            temp[d] = nullptr;
            d++;
            while (d >= static_cast<int>(temp.size())) {
                temp.push_back(nullptr);
            }
        }
        temp[d] = std::move(curr);
    }
    rootList.clear();
    for(int i=0; i<temp.size(); ++i) {
        if(temp[i] != nullptr) {
            rootList.push_back(std::move(temp[i]));
        }
    }
}

template <typename T, typename Predicate>
void BinomialHeap<T, Predicate>::printHeap() const {
    std::cout << "Binomial Heap:\n";
    for (const auto& root : rootList) {
        printTree(root.get(), 0);
        std::cout << "-----\n";
    }
}

template <typename T, typename Predicate>
void BinomialHeap<T, Predicate>::printTree(const BinomialNode<T>* node, int indent) const {
    if(!node) return;
    for(int i=0; i<indent; ++i) {
        std::cout << " ";
    }
    std::cout << node->data << " (deg=" << node->degree << ")\n";
    printTree(node->child.get(), indent + 1);
    printTree(node->sibling.get(), indent);
}
