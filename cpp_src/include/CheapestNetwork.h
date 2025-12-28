#ifndef CHEAPEST_NETWORK_H
#define CHEAPEST_NETWORK_H

#include "Graph.h"
#include <string>
#include <vector>
#include <tuple>

struct MSTResult {
    bool found;
    vector<tuple<string, string, int>> edges; // (city1, city2, weight)
    int totalCost;
    string message;
};

class CheapestNetwork {
public:
    static MSTResult find(Graph& g);
};

#endif // CHEAPEST_NETWORK_H
