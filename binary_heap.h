#include <vector>
#include <iostream>

template <typename T>
class Heap { // specifically, a min heap
public:
    Heap() {
        heap = {};
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
    int length() {
        return heap.size();
    }
    void min_heapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest;
        if(l <= heap.size() && *heap[l] > *heap[i]) {
            smallest = l;
        } else {
            smallest = i;
        }
        if(r <= heap.size() && *heap[r] > *heap[smallest]) { // right might be too big or too small...
            smallest = r;
        }
        if(smallest != i) {
            T* temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            min_heapify(smallest);
        }
    }
    void build_heap() {
        for (int i = (heap.size() / 2) - 1; i >= 0; --i) {
            min_heapify(i);
        }
    }
    void insert(T& elt) { // insert + propagate up if needed
        heap.push_back(&elt);
        int i = heap.size() - 1;
        while(i > 0 && *heap[parent(i)] > *heap[i]) {
            T* temp = heap[i];
            heap[i] = heap[parent(i)];
            heap[parent(i)] = temp;
            i = parent(i);
        }
    }
    T* extract_min() {
        T* val = heap[0];
        heap[0] = heap[heap.size()-1];
        heap.pop_back();
        min_heapify(0);
        return val;
    }
    bool empty() {
        return heap.size() == 0;
    }
private:
    std::vector<T*> heap; // modifies the
};