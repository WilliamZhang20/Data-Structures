#pragma once

#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

template <typename T>
class Trie {
    class Node {
    public:
        std::unordered_map<T, Node*> child_;
        bool isEnd_;
        Node();
    };

private:
    Node root_;

public:
    Trie();
    ~Trie();

    void insert(const std::vector<T>& arr);
    std::optional<T> search(const std::vector<T>& arr);
    bool startsWith(const std::vector<T>& arr);
private:
    void destroyTrie(Node* node);
};

template <typename T>
Trie<T>::~Trie() {
    destroyTrie(&root_);
}

template <typename T>
Trie<T>::Trie() : root_() {}

template <typename T>
Trie<T>::Node::Node() : child_(), isEnd_(false) {}

template <typename T>
void Trie<T>::insert(const std::vector<T>& arr) {
    Node* current = &root_;
    for(const T& val : arr) {
        if (current->child_.find(val) == current->child_.end()) {
            current->child_[val] = new Node();
        }
        current = current->child_[val];
    }
    current->isEnd_ = true;
}

template <typename T>
std::optional<T> Trie<T>::search(const std::vector<T>& arr) {
    Node* curr = &root_;
    for(const T& c : arr) {
        if(curr->child_.find(c) == curr->child_.end()) {
            return std::nullopt;
        }
        curr = curr->child_.at(c);
    }
    if(curr->isEnd_) {
        return arr;
    }
    else {
        return std::nullopt;
    }
}

template <typename T>
bool Trie<T>::startsWith(const std::vector<T>& arr) {
    Node* current = &root_;
    for (const T& c : arr) {
        if (current->child_.find(c) == current->child_.end()) {
            return false;
        }
        current = current->child_.at(c);
    }
    return true;
}

template <typename T>
void Trie<T>::destroyTrie(Node* node) {
    if (node) {
        for (auto const& [key, child] : node->child_) {
            destroyTrie(child);
        }
        delete node;
    }
}