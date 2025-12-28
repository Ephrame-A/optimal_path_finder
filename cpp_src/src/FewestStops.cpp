#include "../include/FewestStops.h"
#include <map>
#include <algorithm>

FewestStopsResult FewestStops::find(Graph& g, string start, string end) {
    FewestStopsResult res;
    res.found = false;
    res.stops = 0;

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

    map<string, bool> visited;
    map<string, string> parent;
    CustomQueue<string> q;

    q.enqueue(start);
    visited[start] = true;

    while (!q.empty()) {
        string u = q.front(); q.dequeue();
        
        if (u == end) {
            res.found = true;
            res.message = "Path found.";
            
            // Reconstruct
            string curr = end;
            while(curr != start) {
                res.path.push_back(curr);
                curr = parent[curr];
            }
            res.path.push_back(start);
            reverse(res.path.begin(), res.path.end());
            res.stops = res.path.size() - 1;
            return res;
        }

        for (auto& e : g.getNeighbors(u)) {
            if (!visited[e.dest]) {
                visited[e.dest] = true;
                parent[e.dest] = u;
                q.enqueue(e.dest);
            }
        }
    }

    res.message = "No path found.";
    return res;
}
