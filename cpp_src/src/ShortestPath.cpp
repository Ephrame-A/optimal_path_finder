#include "../include/ShortestPath.h"
#include <map>
#include <climits>

ShortestPathResult ShortestPath::find(Graph& g, string start, string end) {
    ShortestPathResult res;
    res.found = false;
    res.distance = 0;

    // Check if start and end cities exist in the graph
    vector<string> allNodes = g.getNodes();
    bool startExists = false, endExists = false;
    for (const auto& n : allNodes) {
        if (n == start) startExists = true;
        if (n == end) endExists = true;
    }

    if (!startExists || !endExists) {
        res.message = "One or both cities not found in the network.";
        return res;
    }

    map<string, int> dist;
    map<string, string> parent;
    for (const auto& city : allNodes) {
        dist[city] = INT_MAX;
    }

    MinPQ pq;
    dist[start] = 0;
    pq.push(0, start);

    while (!pq.empty()) {
        PQNode top = pq.pop();
        
        if (top.weight > dist[top.city]) continue;
        if (top.city == end) break;

        for (const auto& edge : g.getNeighbors(top.city)) {
            int newDist = dist[top.city] + edge.weight;
            
            if (newDist < dist[edge.dest]) {
                dist[edge.dest] = newDist;
                parent[edge.dest] = top.city;
                pq.push(newDist, edge.dest);
            }
        }
    }

    if (dist[end] == INT_MAX) {
        res.message = "No route exists between these cities.";
    } else {
        res.found = true;
        res.distance = dist[end];
        
        // Reconstruct path using CustomStack (Person 1's tool)
        CustomStack<string> pathStack;
        string curr = end;
        while (curr != start) {
            pathStack.push(curr);
            curr = parent[curr];
        }
        pathStack.push(start);

        // Transfer from stack to vector
        while (!pathStack.empty()) {
            res.path.push_back(pathStack.top());
            pathStack.pop();
        }
        
        res.message = "Shortest path found successfully.";
    }

    return res;
}
