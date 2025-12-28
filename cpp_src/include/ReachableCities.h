#ifndef REACHABLE_CITIES_H
#define REACHABLE_CITIES_H

#include "Graph.h"
#include <string>
#include <vector>

class ReachableCities {
public:
    static vector<string> find(Graph& g, string start);
};

#endif // REACHABLE_CITIES_H
