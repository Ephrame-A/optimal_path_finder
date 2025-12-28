#include "../include/MultiCityTour.h"
#include <map>
#include <algorithm>

TourResult MultiCityTour::plan(Graph& g, vector<string> cities) {
    TourResult res;
    res.found = false;
    res.totalDistance = 0;

    if (cities.empty()) {
        res.message = "No cities provided.";
        return res;
    }

    if (cities.size() == 1) {
        res.found = true;
        res.path = cities;
        res.totalDistance = 0;
        res.message = "Single city tour.";
        return res;
    }

    // Verify all cities exist
    auto allNodes = g.getNodes();
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

    // Simple greedy nearest neighbor heuristic for TSP
    vector<string> path;
    map<string, bool> visited;
    string current = cities[0];
    path.push_back(current);
    visited[current] = true;
    int totalDist = 0;

    for (size_t i = 1; i < cities.size(); i++) {
        string nearest;
        int minDist = INT_MAX;
        
        for (const auto& city : cities) {
            if (visited[city]) continue;
            
            // Find distance from current to city
            int dist = INT_MAX;
            for (const auto& edge : g.getNeighbors(current)) {
                if (edge.dest == city) {
                    dist = edge.weight;
                    break;
                }
            }
            
            if (dist < minDist) {
                minDist = dist;
                nearest = city;
            }
        }
        
        if (minDist == INT_MAX) {
            res.message = "Cities are not all reachable from each other.";
            return res;
        }
        
        path.push_back(nearest);
        visited[nearest] = true;
        totalDist += minDist;
        current = nearest;
    }

    res.found = true;
    res.path = path;
    res.totalDistance = totalDist;
    res.message = "Tour planned successfully.";
    return res;
}

void MultiCityTour::tspHelper(Graph& g, vector<string>& cities, vector<bool>& visited, 
                              string current, int count, int cost, int& minCost, 
                              vector<string>& currentPath, vector<string>& bestPath) {
    // This is a placeholder for exact TSP - not implemented due to complexity
    // The plan() function uses greedy heuristic instead
}
