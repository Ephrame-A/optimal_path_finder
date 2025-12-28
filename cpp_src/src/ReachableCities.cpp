#include "../include/ReachableCities.h"
#include <map>

vector<string> ReachableCities::find(Graph& g, string start) {
    vector<string> reachable;
    if(!g.getNeighbors(start).empty() || g.hasEdge(start, start)) { // Check if node exists basically
        // Or check g.getNodes()
    }
    // Better check:
    bool exists = false;
    for(const auto& n : g.getNodes()) if(n == start) exists = true;
    if(!exists) return reachable;

    map<string, bool> visited;
    CustomStack<string> s;
    s.push(start);

    while (!s.empty()) {
        string u = s.top(); s.pop();
        if (!visited[u]) {
            visited[u] = true;
            if (u != start) reachable.push_back(u);
            for (auto& e : g.getNeighbors(u)) {
                s.push(e.dest);
            }
        }
    }
    return reachable;
}
