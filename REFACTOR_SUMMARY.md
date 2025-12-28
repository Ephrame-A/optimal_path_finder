# Path Finder Pro - Refactored Project

## Overview
This project has been completely refactored with a modular C++ backend and a beautiful, interactive visualization frontend.

## Project Structure

### C++ Backend (Modular Architecture)
```
cpp_src/
├── include/
│   ├── DataStructures.h      # Custom Stack, Queue, MinPQ, DisjointSet
│   ├── Graph.h                # Graph data structure
│   ├── ShortestPath.h         # Dijkstra's algorithm
│   ├── FewestStops.h          # BFS for minimum hops
│   ├── ReachableCities.h      # DFS for connectivity
│   ├── MultiCityTour.h        # TSP (greedy heuristic)
│   ├── CheapestNetwork.h      # MST (Kruskal's algorithm)
│   └── PathFinder.h           # Main unified interface
└── src/
    ├── Graph.cpp
    ├── ShortestPath.cpp
    ├── FewestStops.cpp
    ├── ReachableCities.cpp
    ├── MultiCityTour.cpp
    ├── CheapestNetwork.cpp
    └── PathFinder.cpp
```

### Frontend
```
travel_chatbot/
├── templates/
│   └── pathfinder.html        # Main UI template
└── static/
    ├── css/
    │   └── pathfinder.css     # Modern, animated styles
    └── js/
        └── pathfinder.js      # Graph visualization & interactivity
```

### Django Backend
```
travel_chatbot/pathfinding_service/
├── views.py                   # API endpoints
└── urls.py                    # URL routing
```

## Key Features

### 1. Find Shortest Path (Dijkstra's Algorithm)
- Finds minimum distance path between two cities
- Uses custom MinPQ for efficient pathfinding

### 2. Build Map (Add/Remove Cities)
- Add bidirectional routes with distances
- Remove routes between cities
- Input validation

### 3. Find Path with Fewest Stops (BFS)
- Minimum number of transitions
- Uses custom Queue implementation

### 4. Plan Multi-City Tour (TSP Heuristic)
- Greedy nearest neighbor algorithm
- Visits all specified cities efficiently

### 5. Display All Reachable Cities (DFS)
- Shows connectivity from a starting point
- Uses custom Stack implementation

### 6. Find Cheapest Network (MST - Kruskal's)
- Minimum Spanning Tree
- Connects all cities with minimum total cost
- Uses Disjoint Set for cycle detection

## UI Features

### Visual Design
- **Modern Gradient Background**: Purple gradient backdrop
- **Glass morphism Effects**: Frosted glass panels with backdrop blur
- **Smooth Animations**: Path pulse, node highlight animations
- **Interactive Canvas**: Drag to pan, scroll to zoom
- **Responsive Layout**: Sidebar + main canvas

### Interactions
- **Click & Drag**: Pan the graph view
- **Mouse Wheel**: Zoom in/out
- **Button Actions**: Each feature has a dedicated button
- **Modal Inputs**: Clean input dialogs for operations
- **Result Panel**: Animated slide-in results display

### Animation Effects
1. **Path Highlighting**: Pulsing animated paths when found
2. **Node Highlighting**: Glowing nodes with radial gradients
3. **Edge Labels**: Weight display on connections
4. **Smooth Transitions**: CSS animations for UI elements

## Building the Project

### Compile C++ Module
```bash
cd /home/ermias/Desktop/path_finder/optimal_path_finder
source venv/bin/activate
python3 setup_new.py build_ext --inplace
cp pathfinder.cpython-312-x86_64-linux-gnu.so travel_chatbot/
```

### Run Django Server
```bash
cd travel_chatbot
source ../venv/bin/activate
python manage.py runserver 0.0.0.0:8000
```

### Access the Application
Open your browser and navigate to:
```
http://localhost:8000/
```

## API Endpoints

- `POST /api/add_route` - Add a route
- `POST /api/remove_route` - Remove a route
- `POST /api/shortest_path` - Find shortest path
- `POST /api/fewest_stops` - Find fewest stops path
- `POST /api/reachable` - Find reachable cities
- `POST /api/multi_city_tour` - Plan multi-city tour
- `GET /api/cheapest_network` - Find MST
- `GET /api/graph` - Get current graph data
- `POST /api/clear` - Clear all data
- `POST /api/load_sample` - Load sample graph

## Code Quality

### Modularity
- **1 File = 1 Functional Requirement**: Each algorithm is in its own file
- **Minimal Built-in Usage**: Custom implementations of Stack, Queue, Priority Queue
- **Clean Interfaces**: Well-defined result structures
- **Separation of Concerns**: C++ logic separate from Python bindings

### Best Practices
- **Error Handling**: Comprehensive validation
- **Const Correctness**: Proper use of const references
- **Modern C++**: C++17 features (structured bindings, auto)
- **No Memory Leaks**: Proper RAII and smart ownership

## Sample Data
The application includes a "Load Sample" button that loads a pre-configured graph of US cities:
- New York, Boston, Philadelphia, Washington, Richmond, Charlotte, Atlanta
- Jacksonville, Pittsburgh, Cleveland, Detroit, Chicago, Milwaukee, Minneapolis

## Technologies Used
- **C++17**: Core pathfinding algorithms
- **pybind11**: Python-C++ bindings
- **Django**: Web framework
- **HTML5 Canvas**: Graph visualization
- **CSS3**: Modern styling with animations
- **Vanilla JavaScript**: No framework dependencies

## Performance
- Efficient graph algorithms with optimal time complexity
- Custom data structures for better performance
- Canvas rendering for smooth animations
- Minimal memory footprint

## Future Enhancements
1. A* algorithm for heuristic pathfinding
2. Dynamic graph layout algorithms (force-directed)
3. Export/import graph data (JSON, CSV)
4. Path history and comparison
5. Real-time multi-user collaboration
6. 3D visualization option
