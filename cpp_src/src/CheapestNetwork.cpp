#include "../include/CheapestNetwork.h"
#include <algorithm>
#include <map>
#include <functional>

MSTResult CheapestNetwork::find(Graph& g) {
    MSTResult res;
    res.found = false;
    res.totalCost = 0;

    auto nodes = g.getNodes();
    if (nodes.empty()) {
        res.message = "Graph is empty.";
        return res;
    }

    // Get all edges and sort by weight (Kruskal's algorithm)
    auto edges = g.getAllEdges();
    sort(edges.begin(), edges.end()); // Sorts by first element (weight)

    // Disjoint set for cycle detection
    map<string, string> parent;
    map<string, int> rank;
    
    auto makeSet = [&](string s) {
        parent[s] = s;
        rank[s] = 0;
    };
    
    function<string(string)> findSet = [&](string s) -> string {
        if (parent.find(s) == parent.end()) makeSet(s);
        if (parent[s] != s) parent[s] = findSet(parent[s]);
        return parent[s];
    };
    
    auto unite = [&](string x, string y) {
        string rootX = findSet(x);
        string rootY = findSet(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) swap(rootX, rootY);
            parent[rootY] = rootX;
            if (rank[rootX] == rank[rootY]) rank[rootX]++;
        }
    };

    // Initialize disjoint sets
    for (const auto& node : nodes) {
        makeSet(node);
    }

    // Kruskal's algorithm
    int edgeCount = 0;
    for (const auto& [weight, u, v] : edges) {
        if (findSet(u) != findSet(v)) {
            unite(u, v);
            res.edges.push_back({u, v, weight});
            res.totalCost += weight;
            edgeCount++;
            
            if (edgeCount == (int)nodes.size() - 1) {
                break; // MST complete
            }
        }
    }

    if (edgeCount == (int)nodes.size() - 1) {
        res.found = true;
        res.message = "MST found successfully.";
    } else {
        res.message = "Graph is not connected.";
    }

    return res;
}
