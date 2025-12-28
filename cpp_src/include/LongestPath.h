#ifndef LONGEST_PATH_H
#define LONGEST_PATH_H

#include "Graph.h"
#include <string>
#include <vector>

struct LongestPathResult {
    bool found;
    vector<string> path;
    int distance;
    string message;
};

class LongestPath {
public:
    static LongestPathResult find(Graph& g, string start, string end);
private:
    static void dfsLongest(Graph& g, string current, string end, 
                          map<string, bool>& visited, vector<string>& currentPath,
                          int currentDist, vector<string>& bestPath, int& maxDist);
};

#endif // LONGEST_PATH_H
