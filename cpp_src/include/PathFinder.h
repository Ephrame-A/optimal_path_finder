#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "Graph.h"
#include "ShortestPath.h"
#include "FewestStops.h"
#include "ReachableCities.h"
#include "MultiCityTour.h"
#include "CheapestNetwork.h"
#include "LongestPath.h"
#include <string>
#include <vector>
#include <tuple>

struct OperationResult {
    bool success;
    string message;
};

class PathFinder {
private:
    Graph graph;

public:
    PathFinder() {}

    // Graph operations
    OperationResult addCity(string city1, string city2, int distance);
    OperationResult removeCity(string city1, string city2);
    
    // Query operations
    ShortestPathResult findShortestPath(string start, string end);
    LongestPathResult findLongestPath(string start, string end);
    FewestStopsResult findFewestStops(string start, string end);
    vector<string> findReachableCities(string start);
    TourResult planMultiCityTour(vector<string> cities);
    MSTResult findCheapestNetwork();
    
    // Get graph data
    vector<string> getAllCities();
    vector<tuple<string, string, int>> getAllRoutes();
    void clearAll();
};

#endif // PATH_FINDER_H
