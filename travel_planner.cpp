#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <climits>
#include <algorithm>

using namespace std;

// --- Custom Linked List based Stack for DFS & Path Reconstruction ---
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

// --- Min-Priority Queue for Dijkstra/Prim ---
struct PQNode { int weight; string city; };
class MinPQ {
    vector<PQNode> heap;
public:
    void push(int w, string c) {
        heap.push_back({w, c});
        push_heap(heap.begin(), heap.end(), [](PQNode a, PQNode b) { return a.weight > b.weight; });
    }
    PQNode pop() {
        pop_heap(heap.begin(), heap.end(), [](PQNode a, PQNode b) { return a.weight > b.weight; });
        PQNode top = heap.back(); heap.pop_back();
        return top;
    }
    bool empty() { return heap.empty(); }
};

struct Edge { string dest; int weight; };

class TravelPlanner {
    map<string, vector<Edge>> adjList;

    // Helper for printing paths from a parent map
    void printPath(map<string, string>& parent, string current) {
        if (parent.find(current) == parent.end()) {
            cout << current;
            return;
        }
        printPath(parent, parent[current]);
        cout << " -> " << current;
    }

public:
    void addRoute(string u, string v, int w) {
        if (w <= 0) {
            cout << "Error: Distance must be positive.\n";
            return;
        }
        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
        cout << "Successfully added: " << u << " <-> " << v << " (" << w << "km)\n";
    }

    void deleteRoute(string u, string v) {
        auto removeEdge = [&](string from, string to) {
            if (adjList.count(from)) {
                for (auto it = adjList[from].begin(); it != adjList[from].end(); ++it) {
                    if (it->dest == to) {
                        adjList[from].erase(it);
                        return true;
                    }
                }
            }
            return false;
        };
        if (removeEdge(u, v) && removeEdge(v, u)) cout << "Route between " << u << " and " << v << " deleted.\n";
        else cout << "Route not found.\n";
    }

    void dijkstra(string start, string end) {
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            cout << "Error: One or both cities do not exist in the map.\n";
            return;
        }

        map<string, int> dist;
        map<string, string> parent;
        for (auto const& [city, _] : adjList) dist[city] = INT_MAX;

        MinPQ pq;
        dist[start] = 0;
        pq.push(0, start);

        while (!pq.empty()) {
            PQNode top = pq.pop();
            if (top.weight > dist[top.city]) continue;
            if (top.city == end) break;

            for (auto& edge : adjList[top.city]) {
                if (dist[top.city] + edge.weight < dist[edge.dest]) {
                    dist[edge.dest] = dist[top.city] + edge.weight;
                    parent[edge.dest] = top.city;
                    pq.push(dist[edge.dest], edge.dest);
                }
            }
        }

        if (dist[end] == INT_MAX) {
            cout << "No route exists between " << start << " and " << end << ".\n";
        } else {
            cout << "Optimal Path: "; printPath(parent, end);
            cout << "\nTotal Distance: " << dist[end] << "km\n";
        }
    }

    void fewestStops(string start, string end) {
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            cout << "Error: Invalid cities.\n";
            return;
        }
        map<string, bool> visited;
        map<string, string> parent;
        CustomQueue<string> q;

        q.enqueue(start);
        visited[start] = true;

        while (!q.empty()) {
            string u = q.front(); q.dequeue();
            if (u == end) {
                cout << "Route with fewest stops: "; printPath(parent, end);
                cout << endl;
                return;
            }
            for (auto& e : adjList[u]) {
                if (!visited[e.dest]) {
                    visited[e.dest] = true;
                    parent[e.dest] = u;
                    q.enqueue(e.dest);
                }
            }
        }
        cout << "Destination unreachable.\n";
    }

    void showMapStats() {
        cout << "\n--- Map Statistics ---\n";
        cout << "Total Cities: " << adjList.size() << endl;
        int routes = 0;
        for (auto const& [city, edges] : adjList) routes += edges.size();
        cout << "Total Routes: " << routes / 2 << endl; // Divide by 2 for undirected
    }

    // Additional features: DFS, TSP, MST logic remains similar but with error checks
    void displayReachable(string start) {
        if (adjList.find(start) == adjList.end()) { cout << "City not found.\n"; return; }
        map<string, bool> visited;
        CustomStack<string> s;
        s.push(start);
        cout << "Reachable: ";
        while (!s.empty()) {
            string u = s.top(); s.pop();
            if (!visited[u]) {
                visited[u] = true;
                cout << u << " ";
                for (auto& e : adjList[u]) s.push(e.dest);
            }
        }
        cout << endl;
    }
};

int main() {
    TravelPlanner planner;
    int choice;
    string c1, c2;
    int w;

    while (true) {
        cout << "\n========== TRAVEL PLANNER PRO ==========\n";
        cout << "1. Add Route\n2. Delete Route\n3. Shortest Path (Dijkstra)\n";
        cout << "4. Fewest Stops (BFS)\n5. Show Reachable (DFS)\n6. Map Statistics\n0. Exit\n";
        cout << "Selection: ";

        if (!(cin >> choice)) { // Robustness: Handle non-integer input
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }

        if (choice == 0) break;
        switch (choice) {
            case 1:
                cout << "Source, Dest, Weight: "; cin >> c1 >> c2 >> w;
                planner.addRoute(c1, c2, w);
                break;
            case 2:
                cout << "Enter the two cities: "; cin >> c1 >> c2;
                planner.deleteRoute(c1, c2);
                break;
            case 3:
                cout << "From, To: "; cin >> c1 >> c2;
                planner.dijkstra(c1, c2);
                break;
            case 4:
                cout << "From, To: "; cin >> c1 >> c2;
                planner.fewestStops(c1, c2);
                break;
            case 5:
                cout << "Start City: "; cin >> c1;
                planner.displayReachable(c1);
                break;
            case 6:
                planner.showMapStats();
                break;
            default: cout << "Choice out of range.\n";
        }
    }
    return 0;
}
