#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "Graph.h"
#include <string>
#include <vector>

struct ShortestPathResult {
    bool found;
    vector<string> path;
    int distance;
    string message;
};

class ShortestPath {
public:
    static ShortestPathResult find(Graph& g, string start, string end);
};

#endif // SHORTEST_PATH_H
