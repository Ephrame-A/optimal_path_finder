#ifndef TRAVEL_PLANNER_LIB_H
#define TRAVEL_PLANNER_LIB_H

#include <vector>
#include <map>
#include <string>
#include <climits>
#include <algorithm>
#include <sstream>
#include <memory>

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

// Result structures for returning data instead of printing
struct PathResult {
    bool success;
    string message;
    vector<string> path;
    int distance;
    
    PathResult() : success(false), distance(0) {}
};

struct RouteOperationResult {
    bool success;
    string message;
    
    RouteOperationResult() : success(false) {}
};

struct MapStats {
    int totalCities;
    int totalRoutes;
};

class TravelPlannerLib {
private:
    map<string, vector<Edge>> adjList;

    // Helper to build path from parent map
    vector<string> buildPath(map<string, string>& parent, const string& current) {
        vector<string> path;
        string node = current;
        
        while (parent.find(node) != parent.end()) {
            path.push_back(node);
            node = parent[node];
        }
        path.push_back(node);
        
        reverse(path.begin(), path.end());
        return path;
    }

public:
    TravelPlannerLib() {}

    // Add a route between two cities
    RouteOperationResult addRoute(const string& u, const string& v, int w) {
        RouteOperationResult result;
        
        if (w <= 0) {
            result.success = false;
            result.message = "Error: Distance must be positive.";
            return result;
        }
        
        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
        
        result.success = true;
        result.message = "Successfully added: " + u + " <-> " + v + " (" + to_string(w) + "km)";
        return result;
    }

    // Delete a route between two cities
    RouteOperationResult deleteRoute(const string& u, const string& v) {
        RouteOperationResult result;
        
        auto removeEdge = [&](const string& from, const string& to) -> bool {
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
        
        if (removeEdge(u, v) && removeEdge(v, u)) {
            result.success = true;
            result.message = "Route between " + u + " and " + v + " deleted.";
        } else {
            result.success = false;
            result.message = "Route not found.";
        }
        
        return result;
    }

    // Find shortest path using Dijkstra's algorithm
    PathResult findShortestPath(const string& start, const string& end) {
        PathResult result;
        
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            result.success = false;
            result.message = "Error: One or both cities do not exist in the map.";
            return result;
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
            result.success = false;
            result.message = "No route exists between " + start + " and " + end + ".";
        } else {
            result.success = true;
            result.path = buildPath(parent, end);
            result.distance = dist[end];
            result.message = "Path found successfully.";
        }
        
        return result;
    }

    // Find path with fewest stops using BFS
    PathResult findFewestStops(const string& start, const string& end) {
        PathResult result;
        
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            result.success = false;
            result.message = "Error: Invalid cities.";
            return result;
        }
        
        map<string, bool> visited;
        map<string, string> parent;
        CustomQueue<string> q;

        q.enqueue(start);
        visited[start] = true;

        while (!q.empty()) {
            string u = q.front(); 
            q.dequeue();
            
            if (u == end) {
                result.success = true;
                result.path = buildPath(parent, end);
                result.message = "Route with fewest stops found.";
                result.distance = result.path.size() - 1; // Number of hops
                return result;
            }
            
            for (auto& e : adjList[u]) {
                if (!visited[e.dest]) {
                    visited[e.dest] = true;
                    parent[e.dest] = u;
                    q.enqueue(e.dest);
                }
            }
        }
        
        result.success = false;
        result.message = "Destination unreachable.";
        return result;
    }

    // Get all reachable cities from a starting point
    vector<string> getReachableCities(const string& start) {
        vector<string> reachable;
        
        if (adjList.find(start) == adjList.end()) {
            return reachable;
        }
        
        map<string, bool> visited;
        CustomStack<string> s;
        s.push(start);
        
        while (!s.empty()) {
            string u = s.top();
            s.pop();
            
            if (!visited[u]) {
                visited[u] = true;
                if (u != start) {  // Don't include the starting city
                    reachable.push_back(u);
                }
                for (auto& e : adjList[u]) {
                    s.push(e.dest);
                }
            }
        }
        
        return reachable;
    }

    // Get map statistics
    MapStats getMapStats() {
        MapStats stats;
        stats.totalCities = adjList.size();
        stats.totalRoutes = 0;
        
        for (auto const& [city, edges] : adjList) {
            stats.totalRoutes += edges.size();
        }
        stats.totalRoutes /= 2; // Divide by 2 for undirected graph
        
        return stats;
    }

    // Get all cities
    vector<string> getAllCities() {
        vector<string> cities;
        for (auto const& [city, _] : adjList) {
            cities.push_back(city);
        }
        return cities;
    }

    // Get all routes
    vector<tuple<string, string, int>> getAllRoutes() {
        vector<tuple<string, string, int>> routes;
        map<pair<string, string>, bool> added;
        
        for (auto const& [city, edges] : adjList) {
            for (auto const& edge : edges) {
                string u = city;
                string v = edge.dest;
                
                // Ensure we only add each undirected edge once
                if (u > v) swap(u, v);
                
                pair<string, string> key = {u, v};
                if (!added[key]) {
                    routes.push_back({u, v, edge.weight});
                    added[key] = true;
                }
            }
        }
        
        return routes;
    }

    // Clear all data
    void clear() {
        adjList.clear();
    }
};

#endif // TRAVEL_PLANNER_LIB_H
