#include "../include/Graph.h"

void Graph::addEdge(string u, string v, int w) {
    // Check if edge exists to update or add
    // For simplicity, we assume we append. But we should probably check duplicates.
    // The previous implementation didn't check duplicates strictly but allowed them in the list? 
    // Wait, the previous one did "adjList[u].push_back".
    // I will add a check to avoid duplicate edges between same nodes to keep it clean.
    if (hasEdge(u, v)) removeEdge(u, v);
    
    adjList[u].push_back({v, w});
    adjList[v].push_back({u, w});
}

void Graph::removeEdge(string u, string v) {
    if (adjList.count(u)) {
        auto& edges = adjList[u];
        for (auto it = edges.begin(); it != edges.end(); ) {
            if (it->dest == v) it = edges.erase(it);
            else ++it;
        }
    }
    if (adjList.count(v)) {
        auto& edges = adjList[v];
        for (auto it = edges.begin(); it != edges.end(); ) {
            if (it->dest == u) it = edges.erase(it);
            else ++it;
        }
    }
}

bool Graph::hasEdge(string u, string v) {
    if (adjList.find(u) == adjList.end()) return false;
    for (const auto& e : adjList[u]) {
        if (e.dest == v) return true;
    }
    return false;
}

vector<Edge> Graph::getNeighbors(string u) {
    if (adjList.find(u) != adjList.end()) return adjList[u];
    return {};
}

vector<string> Graph::getNodes() {
    vector<string> nodes;
    for (auto const& [city, _] : adjList) {
        nodes.push_back(city);
    }
    return nodes;
}

void Graph::clear() {
    adjList.clear();
}

int Graph::getCityCount() {
    return adjList.size();
}

vector<tuple<int, string, string>> Graph::getAllEdges() {
    vector<tuple<int, string, string>> edges;
    map<string, bool> visited; // Using visited to avoid duplicates in undirected graph?
    // Actually simpler: iterate and only add if u < v
    for (auto const& [u, list] : adjList) {
        for (auto const& e : list) {
            if (u < e.dest) {
                edges.push_back({e.weight, u, e.dest});
            }
        }
    }
    return edges;
}
