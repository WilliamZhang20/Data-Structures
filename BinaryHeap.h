#include <vector>
#include <iostream>
#include <functional>
#include <cassert>

template <typename T, typename Predicate = std::greater<T>>
class BinaryHeap {
public:
    BinaryHeap() = default;
    BinaryHeap(std::vector<T> vec) {
        heap = vec;
        build_heap();
    }
    int length() {
        return heap.size();
    }
    void build_heap() {
        for (int i = (heap.size() / 2) - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }
    T remove(int i) {
        T val = heap[i];
        heap[i] = heap[heap.size() - 1];
        heap.pop_back();
        heapifyDown(i);
        return val;
    }
    T& top(int i) {
        assert(!heap.size().empty());
        return heap[0];
    }
    template <typename ...Args>
    void insert(Args&&... args) {
        heap.emplace_back(std::forward<Args>(args)...);
        heapifyUp(heap.size() - 1);
    }
    T extract_min() {
        return remove(0);
    }
    bool empty() {
        return heap.size() == 0;
    }
    void print() {
        for(T& val: heap) {
            std::cout << val << " ";
        } std::cout << std::endl;
    }
private:
    std::vector<T> heap; // heap contents in dynamic array

    void heapifyUp(int i, Predicate p = Predicate()) {
        while(i >= 0 && p(heap[parent(i)], heap[i])) { // if parent bigger move parent down and swap
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    int left(int i) {
        return 2*i+1;
    }
    int right(int i) {
        return 2*i+2;
    }
    int parent(int i) {
        return (i - 1) / 2;
    }

    void heapifyDown(int i, Predicate p = Predicate()) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if(l < heap.size() && p(heap[i], heap[l])) {
            smallest = l;
        } else {
            smallest = i;
        }
        if(r < heap.size() && p(heap[smallest], heap[r])) { // right might be too big or too small...
            smallest = r;
        }
        if(smallest != i) {
            std::swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }
};
