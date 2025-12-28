#include "../include/LongestPath.h"
#include <map>
#include <algorithm>

void LongestPath::dfsLongest(Graph& g, string current, string end, 
                             map<string, bool>& visited, vector<string>& currentPath,
                             int currentDist, vector<string>& bestPath, int& maxDist) {
    if (current == end) {
        if (currentDist > maxDist) {
            maxDist = currentDist;
            bestPath = currentPath;
        }
        return;
    }
    
    for (const auto& edge : g.getNeighbors(current)) {
        if (!visited[edge.dest]) {
            visited[edge.dest] = true;
            currentPath.push_back(edge.dest);
            
            dfsLongest(g, edge.dest, end, visited, currentPath, 
                      currentDist + edge.weight, bestPath, maxDist);
            
            currentPath.pop_back();
            visited[edge.dest] = false;
        }
    }
}

LongestPathResult LongestPath::find(Graph& g, string start, string end) {
    LongestPathResult res;
    res.found = false;
    res.distance = 0;

    auto nodes = g.getNodes();
    bool startExists = false, endExists = false;
    for(const auto& n : nodes) {
        if(n == start) startExists = true;
        if(n == end) endExists = true;
    }

    if(!startExists || !endExists) {
        res.message = "One or both cities not found.";
        return res;
    }

    if (start == end) {
        res.message = "Start and end are the same city.";
        return res;
    }

    map<string, bool> visited;
    vector<string> currentPath;
    vector<string> bestPath;
    int maxDist = -1;

    visited[start] = true;
    currentPath.push_back(start);

    dfsLongest(g, start, end, visited, currentPath, 0, bestPath, maxDist);

    if (maxDist >= 0) {
        res.found = true;
        res.path = bestPath;
        res.distance = maxDist;
        res.message = "Longest path found.";
    } else {
        res.message = "No path found.";
    }

    return res;
}
