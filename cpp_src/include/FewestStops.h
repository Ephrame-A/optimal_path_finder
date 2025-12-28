#ifndef FEWEST_STOPS_H
#define FEWEST_STOPS_H

#include "Graph.h"
#include <string>
#include <vector>

struct FewestStopsResult {
    bool found;
    vector<string> path;
    int stops;
    string message;
};

class FewestStops {
public:
    static FewestStopsResult find(Graph& g, string start, string end);
};

#endif // FEWEST_STOPS_H
