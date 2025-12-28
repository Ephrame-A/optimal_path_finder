#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cpp_src/include/PathFinder.h"

namespace py = pybind11;

PYBIND11_MODULE(pathfinder, m) {
    m.doc() = "Modular Path Finder Engine";

    // OperationResult
    py::class_<OperationResult>(m, "OperationResult")
        .def(py::init<>())
        .def_readwrite("success", &OperationResult::success)
        .def_readwrite("message", &OperationResult::message);

    // ShortestPathResult structure
    py::class_<ShortestPathResult>(m, "ShortestPathResult")
        .def(py::init<>())
        .def_readwrite("found", &ShortestPathResult::found)
        .def_readwrite("path", &ShortestPathResult::path)
        .def_readwrite("distance", &ShortestPathResult::distance)
        .def_readwrite("message", &ShortestPathResult::message);

    // LongestPathResult structure
    py::class_<LongestPathResult>(m, "LongestPathResult")
        .def(py::init<>())
        .def_readwrite("found", &LongestPathResult::found)
        .def_readwrite("path", &LongestPathResult::path)
        .def_readwrite("distance", &LongestPathResult::distance)
        .def_readwrite("message", &LongestPathResult::message);

    // FewestStopsResult
    py::class_<FewestStopsResult>(m, "FewestStopsResult")
        .def(py::init<>())
        .def_readwrite("found", &FewestStopsResult::found)
        .def_readwrite("path", &FewestStopsResult::path)
        .def_readwrite("stops", &FewestStopsResult::stops)
        .def_readwrite("message", &FewestStopsResult::message);

    // TourResult
    py::class_<TourResult>(m, "TourResult")
        .def(py::init<>())
        .def_readwrite("found", &TourResult::found)
        .def_readwrite("path", &TourResult::path)
        .def_readwrite("totalDistance", &TourResult::totalDistance)
        .def_readwrite("message", &TourResult::message);

    // MSTResult
    py::class_<MSTResult>(m, "MSTResult")
        .def(py::init<>())
        .def_readwrite("found", &MSTResult::found)
        .def_readwrite("edges", &MSTResult::edges)
        .def_readwrite("totalCost", &MSTResult::totalCost)
        .def_readwrite("message", &MSTResult::message);

    // PathFinder class
    py::class_<PathFinder>(m, "PathFinder")
        .def(py::init<>())
        .def("add_city", &PathFinder::addCity,
             "Add a route between two cities",
             py::arg("city1"), py::arg("city2"), py::arg("distance"))
        .def("remove_city", &PathFinder::removeCity,
             "Remove a route between two cities",
             py::arg("city1"), py::arg("city2"))
        .def("find_shortest_path", &PathFinder::findShortestPath,
             "Find the shortest path between two cities using Dijkstra's algorithm",
             py::arg("start"), py::arg("end"))
        .def("find_longest_path", &PathFinder::findLongestPath,
             "Find the longest simple path between two cities using DFS",
             py::arg("start"), py::arg("end"))
        .def("find_fewest_stops", &PathFinder::findFewestStops,
             "Find path with fewest stops using BFS",
             py::arg("start"), py::arg("end"))
        .def("find_reachable_cities", &PathFinder::findReachableCities,
             "Find all reachable cities from start",
             py::arg("start"))
        .def("plan_multi_city_tour", &PathFinder::planMultiCityTour,
             "Plan a multi-city tour",
             py::arg("cities"))
        .def("find_cheapest_network", &PathFinder::findCheapestNetwork,
             "Find the cheapest network (MST)")
        .def("get_all_cities", &PathFinder::getAllCities,
             "Get all cities in the graph")
        .def("get_all_routes", &PathFinder::getAllRoutes,
             "Get all routes in the graph")
        .def("clear_all", &PathFinder::clearAll,
             "Clear all data");
}
