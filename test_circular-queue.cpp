#include "CircularQueue.h"

int main() {
    CircularQueue<int> q;

    // Test enqueue
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    q.enQueue(4);
    std::cout << "Initial queue front: " << q.Front() << ", rear: " << q.Rear() << "\n";

    // Test dequeue
    q.deQueue();
    std::cout << "After one dequeue, front: " << q.Front() << ", rear: " << q.Rear() << "\n";

    // Test push_range (variadic template)
    q.push_range(5, 6, 7);
    std::cout << "After push_range(5,6,7), size: " << q.size() << "\n";

    // Print all elements by dequeueing
    std::cout << "Queue contents: ";
    while (!q.isEmpty()) {
        std::cout << q.Front() << " ";
        q.deQueue();
    }
    std::cout << "\n";
    // Queue empty now...
    
    // Test copy constructor
    q.push_range(10, 20, 30);

    CircularQueue<int> q2 = q;  // copy constructor
    std::cout << "Copied queue contents: ";
    while (!q2.isEmpty()) {
        std::cout << q2.Front() << " ";
        q2.deQueue();
    }
    std::cout << "\n";

    // Test move constructor
    q.push_range(100, 200);
    CircularQueue<int> q3 = std::move(q);
    std::cout << "Moved queue contents: ";
    while (!q3.isEmpty()) {
        std::cout << q3.Front() << " ";
        q3.deQueue();
    }
    std::cout << "\n";

    CircularQueue<std::pair<int, std::string>> pq;

    // Emplace some elements
    pq.emplace(1, "one");
    pq.emplace(2, "two");
    pq.emplace(3, "three");
    std::cout << "Emplaced pairs:\n";
    while (!pq.isEmpty()) {
        auto p = pq.Front();
        std::cout << "(" << p.first << ", " << p.second << ") ";
        pq.deQueue();
    }
    std::cout << "\n";
    return 0;
}
