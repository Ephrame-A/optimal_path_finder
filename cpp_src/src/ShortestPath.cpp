#include "../include/ShortestPath.h"
#include <map>
#include <algorithm>

ShortestPathResult ShortestPath::find(Graph& g, string start, string end) {
    ShortestPathResult res;
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

    map<string, int> dist;
    map<string, string> parent;
    for (const auto& city : nodes) dist[city] = INT_MAX;

    MinPQ pq;
    dist[start] = 0;
    pq.push(0, start);

    while (!pq.empty()) {
        PQNode top = pq.pop();
        if (top.weight > dist[top.city]) continue;
        if (top.city == end) break;

        for (auto& edge : g.getNeighbors(top.city)) {
            if (dist[top.city] + edge.weight < dist[edge.dest]) {
                dist[edge.dest] = dist[top.city] + edge.weight;
                parent[edge.dest] = top.city;
                pq.push(dist[edge.dest], edge.dest);
            }
        }
    }

    if (dist[end] == INT_MAX) {
        res.message = "No path found.";
    } else {
        res.found = true;
        res.distance = dist[end];
        
        // Reconstruct path
        string curr = end;
        while(curr != start) {
            res.path.push_back(curr);
            curr = parent[curr];
        }
        res.path.push_back(start);
        reverse(res.path.begin(), res.path.end());
        res.message = "Path found.";
    }

    return res;
}
