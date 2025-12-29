#include "../include/Graph.h"

void Graph::addEdge(string u, string v, int w) {
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
    for (const auto& pair : adjList) {
        nodes.push_back(pair.first);
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
    
    for (const auto& pair : adjList) {
        string u = pair.first;
        for (const auto& e : pair.second) {
            if (u < e.dest) {
                edges.push_back(make_tuple(e.weight, u, e.dest));
            }
        }
    }
    return edges;
}
