#ifndef MULTI_CITY_TOUR_H
#define MULTI_CITY_TOUR_H

#include "Graph.h"
#include <string>
#include <vector>

struct TourResult {
    bool found;
    vector<string> path;
    int totalDistance;
    string message;
};

class MultiCityTour {
public:
    static TourResult plan(Graph& g, vector<string> cities);
private:
    static void tspHelper(Graph& g, vector<string>& cities, vector<bool>& visited, 
                         string current, int count, int cost, int& minCost, 
                         vector<string>& currentPath, vector<string>& bestPath);
};

#endif // MULTI_CITY_TOUR_H
