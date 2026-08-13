#pragma once
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Task.h"

// Binary min-heap of Tasks, ordered by `priority` (lower = more urgent).
// Also maintains an id -> index map so a task can be found and removed
// by id in O(log n) instead of the O(n) a plain heap would need.
class MinHeap {
public:
    void push(const Task& task) {
        if (idToIndex.count(task.id)) {
            throw std::invalid_argument("Task id already exists: " + std::to_string(task.id));
        }
        heap.push_back(task);
        size_t idx = heap.size() - 1;
        idToIndex[task.id] = idx;
        bubbleUp(idx);
    }

    // Removes and returns the highest-priority (lowest value) task.
    Task extractMin() {
        if (heap.empty()) {
            throw std::out_of_range("extractMin() called on empty heap");
        }
        Task top = heap[0];
        swapNodes(0, heap.size() - 1);
        idToIndex.erase(top.id);
        heap.pop_back();
        if (!heap.empty()) {
            bubbleDown(0);
        }
        return top;
    }

    const Task& peek() const {
        if (heap.empty()) {
            throw std::out_of_range("peek() called on empty heap");
        }
        return heap[0];
    }

    // Removes a task by id from anywhere in the heap. O(log n).
    bool remove(int id) {
        auto it = idToIndex.find(id);
        if (it == idToIndex.end()) {
            return false; // not found — nothing to cancel
        }
        size_t idx = it->second;
        size_t lastIdx = heap.size() - 1;
        swapNodes(idx, lastIdx);
        idToIndex.erase(id);
        heap.pop_back();

        if (idx < heap.size()) {
            // The node that moved into `idx` might need to move either way.
            bubbleDown(idx);
            bubbleUp(idx);
        }
        return true;
    }

    bool contains(int id) const {
        return idToIndex.count(id) > 0;
    }

    bool isEmpty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }

    const std::vector<Task>& all() const { return heap; }

private:
    std::vector<Task> heap;
    std::unordered_map<int, size_t> idToIndex;

    static size_t parent(size_t i) { return (i - 1) / 2; }
    static size_t left(size_t i)   { return 2 * i + 1; }
    static size_t right(size_t i)  { return 2 * i + 2; }

    void swapNodes(size_t i, size_t j) {
        std::swap(heap[i], heap[j]);
        idToIndex[heap[i].id] = i;
        idToIndex[heap[j].id] = j;
    }

    void bubbleUp(size_t i) {
        while (i > 0 && heap[i].priority < heap[parent(i)].priority) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    void bubbleDown(size_t i) {
        size_t n = heap.size();
        while (true) {
            size_t smallest = i;
            size_t l = left(i), r = right(i);
            if (l < n && heap[l].priority < heap[smallest].priority) smallest = l;
            if (r < n && heap[r].priority < heap[smallest].priority) smallest = r;
            if (smallest == i) break;
            swapNodes(i, smallest);
            i = smallest;
        }
    }
};
