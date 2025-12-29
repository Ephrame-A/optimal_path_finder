#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <map>

using namespace std;

// --- Custom Linked List based Stack for DFS & Reconstruct Path ---
template <typename T>
class CustomStack {
private:
    struct Node { T data; Node* next; };
    Node* topNode;
public:
    CustomStack() : topNode(nullptr) {}
    void push(T val) { topNode = new Node{val, topNode}; }
    void pop() {
        if (topNode) {
            Node* t = topNode;
            topNode = topNode->next;
            delete t;
        }
    }
    T top() { return topNode->data; }
    bool empty() { return topNode == nullptr; }
    ~CustomStack() { while (!empty()) pop(); }
};

// --- Custom Linked List based Queue for BFS ---
template <typename T>
class CustomQueue {
private:
    struct Node { T data; Node* next; };
    Node *frontNode, *rearNode;
public:
    CustomQueue() : frontNode(nullptr), rearNode(nullptr) {}
    void enqueue(T val) {
        Node* newNode = new Node{val, nullptr};
        if (!rearNode) { frontNode = rearNode = newNode; return; }
        rearNode->next = newNode; rearNode = newNode;
    }
    void dequeue() {
        if (frontNode) {
            Node* t = frontNode;
            frontNode = frontNode->next;
            if (!frontNode) rearNode = nullptr;
            delete t;
        }
    }
    T front() { return frontNode->data; }
    bool empty() { return frontNode == nullptr; }
    ~CustomQueue() { while (!empty()) dequeue(); }
};

// --- Min-Priority Queue
struct PQNode { int weight; string city; };

class MinPQ {
    vector<PQNode> heap;

    // Helper: Move a node up to its correct position
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].weight < heap[parent].weight) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Helper: Move a node down to its correct position
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left].weight < heap[smallest].weight)
                smallest = left;
            if (right < size && heap[right].weight < heap[smallest].weight)
                smallest = right;

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    void push(int w, string c) {
        heap.push_back({w, c});
        heapifyUp(heap.size() - 1);
    }

    PQNode pop() {
        if (heap.empty()) return {0, ""}; // Safety check
        
        PQNode top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return top;
    }

    bool empty() { return heap.empty(); }
};

// --- Disjoint Set for MST (Kruskal's) ---
class DisjointSet {
    map<string, string> parent;
    map<string, int> rank;
public:
    void makeSet(string s) {
        parent[s] = s;
        rank[s] = 0;
    }
    string find(string s) {
        if (parent.find(s) == parent.end()) makeSet(s);
        if (parent[s] != s) parent[s] = find(parent[s]);
        return parent[s];
    }
    void unite(string x, string y) {
        string rootX = find(x);
        string rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) swap(rootX, rootY);
            parent[rootY] = rootX;
            if (rank[rootX] == rank[rootY]) rank[rootX]++;
        }
    }
};

#endif // DATA_STRUCTURES_H
