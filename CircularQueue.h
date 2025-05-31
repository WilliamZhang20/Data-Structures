#pragma once
#include <iostream>
#include <utility>

constexpr int initSize = 4;

template <typename T>
class CircularQueue {
    T* queue;
    int front_;
    int back_;
    int size_;
    int capacity_;
public:
    CircularQueue(int size = initSize);
    CircularQueue(CircularQueue& rhs);
    CircularQueue(CircularQueue&& rhs);
    CircularQueue& operator=(const CircularQueue& rhs);
    CircularQueue& operator=(CircularQueue&& rhs);
    ~CircularQueue();

    template <class ...Args>
    void push_range(Args&&... args);

    template <class ...Args>
    decltype(auto) emplace(Args&&...args);

    void enQueue(T val);
    void deQueue();

    int size() const;

    T Front();
    T Rear();
    
    bool isFull();
    bool isEmpty();
};

template <typename T>
CircularQueue<T>::CircularQueue(int size) {
    queue = new T[size];
    capacity_ = size;
    this->size_ = 0;
    front_ = 0;
    back_ = -1;
}

template <typename T>
CircularQueue<T>::CircularQueue(CircularQueue& rhs) 
: front_(rhs.front_), back_(rhs.back_), size_(rhs.size_), capacity_(rhs.capacity_)
{
    // creating deep copy of rhs
    this->queue = new T[capacity_];
    for (int i = 0; i < size_; ++i) {
        std::cout << rhs.queue[(rhs.front_ + i) % rhs.capacity_] << "\n";
        this->queue[i] = rhs.queue[(rhs.front_ + i) % rhs.capacity_];
    }
    front_ = 0;
    back_ = size_-1;
}

template <typename T>
CircularQueue<T>::CircularQueue(CircularQueue&& rhs) : 
queue(std::exchange(rhs.queue, nullptr)), front_(rhs.front_), back_(rhs.back_), size_(rhs.size_), capacity_(rhs.capacity_)
{
}


template <typename T>
CircularQueue<T>& CircularQueue<T>::operator=(const CircularQueue& rhs) {
    if (this == &rhs) {
        return *this; // self-assignment guard
    }

    // Allocate new memory for copy
    T* newQueue = new T[rhs.capacity_];

    // Copy elements from rhs in correct order (considering circular indexing)
    for (int i = 0; i < rhs.size_; ++i) {
        newQueue[i] = rhs.queue[(rhs.front_ + i) % rhs.capacity_];
    }

    // Delete old queue
    delete[] queue;

    // Assign new values
    queue = newQueue;
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    front_ = 0;
    back_ = size_ - 1;

    return *this;
}

template <typename T>
CircularQueue<T>& CircularQueue<T>::operator=(CircularQueue&& rhs) {
    if(this != rhs) {
        delete[] queue;
        queue = std::exchange(rhs.queue, nullptr);
        front_ = rhs.front_;
        back_ = rhs.back_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;

        rhs.front_ = 0;
        rhs.back_ = 0;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
    }
    return *this;
}

template <typename T>
CircularQueue<T>::~CircularQueue() {
    delete[] queue;
}

template <typename T>
template <typename ...Args>
void CircularQueue<T>::push_range(Args&&... args) {
    (enQueue(std::forward<Args>(args)), ...);
}

template <typename T>
template <class ...Args>
decltype(auto) CircularQueue<T>::emplace(Args&&... args) {
    if (size_ == capacity_) {
        capacity_ = capacity_ * 2 + 1;

        T* b = static_cast<T*>(malloc(capacity_ * sizeof(T)));

        for (int i = 0; i < size_; ++i) {
            new (b + i) T(std::move_if_noexcept(queue[(front_ + i) % capacity_]));
        }

        // Split loop for destructors ensuring exception safety
        for (int i = 0; i < size_; ++i) {
            queue[(front_ + i) % capacity_].~T();
        }
        
        free(queue);
        queue = b;
        front_ = 0;
        back_ = size_ - 1;
    }

    back_ = (back_ + 1) % capacity_;
    size_ += 1;

    new (queue + back_) T(std::forward<Args>(args)...);

    return queue[back_];
}

template <typename T>
void CircularQueue<T>::enQueue(T val) {
    if(size_ == capacity_) {
        capacity_ *= 2;
        ++capacity_;
        T* b = static_cast<T*>(malloc(capacity_ * sizeof(T)));

        for(int i=0; i<size_; ++i) {
            new (b + i) T{std::move_if_noexcept(queue[(front_+i) % size_])};
        }

        delete queue;
        queue = b;

        front_ = 0;
        back_ = size_-1;
    }
    size_ += 1;
    back_ = (back_ + 1) % capacity_;
    new (queue + back_) T{val};
}

template <typename T>
void CircularQueue<T>::deQueue() {
    if (size_ == 0) {
        throw std::out_of_range("Queue is empty");
    }
    front_ = (front_ + 1) % capacity_;
    size_ -= 1;
}

template <typename T>
int CircularQueue<T>::size() const {
    return size_;
}

template <typename T>
T CircularQueue<T>::Front() {
    if (size_ == 0) {
        throw std::out_of_range("Queue is empty");
    }
    return queue[front_];
}

template <typename T>
T CircularQueue<T>::Rear() {
    if (size_ == 0) {
        throw std::out_of_range("Queue is empty");
    }
    return queue[back_];
}

template <typename T>
bool CircularQueue<T>::isEmpty() {
    return size_ == 0;
}

template <typename T>
bool CircularQueue<T>::isFull() {
    return size_ == capacity_;
}
