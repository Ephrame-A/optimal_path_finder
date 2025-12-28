#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <string>
#include <tuple>
#include "DataStructures.h"

struct Edge {
    string dest;
    int weight;
};

class Graph {
private:
    map<string, vector<Edge>> adjList;

public:
    void addEdge(string u, string v, int w);
    void removeEdge(string u, string v);
    bool hasEdge(string u, string v);
    vector<Edge> getNeighbors(string u);
    vector<string> getNodes();
    void clear();
    int getCityCount();
    // Helper to get all edges for MST
    vector<tuple<int, string, string>> getAllEdges();
};

#endif // GRAPH_H
