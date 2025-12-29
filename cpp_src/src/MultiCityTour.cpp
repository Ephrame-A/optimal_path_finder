#include "../include/MultiCityTour.h"
#include <map>
#include <climits>

void MultiCityTour::tspHelper(Graph& g, vector<string>& cities, vector<bool>& visited, 
                              string current, int count, int cost, int& minCost, 
                              vector<string>& currentPath, vector<string>& bestPath) {
    if (count == (int)cities.size()) {
        if (cost < minCost) {
            minCost = cost;
            bestPath = currentPath;
        }
        return;
    }

    for (size_t i = 0; i < cities.size(); ++i) {
        if (!visited[i]) {
            int distToNext = -1;
            vector<Edge> neighbors = g.getNeighbors(current);
            for (const auto& edge : neighbors) {
                if (edge.dest == cities[i]) {
                    distToNext = edge.weight;
                    break;
                }
            }

            if (distToNext != -1) {
                visited[i] = true;
                currentPath.push_back(cities[i]);
                
                tspHelper(g, cities, visited, cities[i], count + 1, 
                          cost + distToNext, minCost, currentPath, bestPath);
                
                currentPath.pop_back();
                visited[i] = false;
            }
        }
    }
}

TourResult MultiCityTour::plan(Graph& g, vector<string> cities) {
    TourResult res;
    res.found = false;
    res.totalDistance = 0;

    if (cities.empty()) {
        res.message = "No cities provided.";
        return res;
    }

    // Verify all cities exist
    vector<string> allNodes = g.getNodes();
    for (const auto& city : cities) {
        bool exists = false;
        for (const auto& node : allNodes) {
            if (node == city) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            res.message = "City '" + city + "' not found in graph.";
            return res;
        }
    }

    int minCost = INT_MAX;
    vector<string> bestPath;
    vector<string> currentPath;
    vector<bool> visited(cities.size(), false);

    // Start from the first city in the list
    visited[0] = true;
    currentPath.push_back(cities[0]);

    tspHelper(g, cities, visited, cities[0], 1, 0, minCost, currentPath, bestPath);

    if (minCost != INT_MAX) {
        res.found = true;
        res.path = bestPath;
        res.totalDistance = minCost;
        res.message = "Optimal tour planned successfully.";
    } else {
        res.message = "Could not find a path visiting all specified cities.";
    }

    return res;
}
