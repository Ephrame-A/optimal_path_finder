#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "travel_planner_lib.h"

namespace py = pybind11;

PYBIND11_MODULE(pathfinding, m) {
    m.doc() = "Travel Planner C++ Pathfinding Engine";

    // PathResult structure
    py::class_<PathResult>(m, "PathResult")
        .def(py::init<>())
        .def_readwrite("success", &PathResult::success)
        .def_readwrite("message", &PathResult::message)
        .def_readwrite("path", &PathResult::path)
        .def_readwrite("distance", &PathResult::distance);

    // RouteOperationResult structure
    py::class_<RouteOperationResult>(m, "RouteOperationResult")
        .def(py::init<>())
        .def_readwrite("success", &RouteOperationResult::success)
        .def_readwrite("message", &RouteOperationResult::message);

    // MapStats structure
    py::class_<MapStats>(m, "MapStats")
        .def(py::init<>())
        .def_readwrite("totalCities", &MapStats::totalCities)
        .def_readwrite("totalRoutes", &MapStats::totalRoutes);

    // TravelPlannerLib class
    py::class_<TravelPlannerLib>(m, "TravelPlannerLib")
        .def(py::init<>())
        .def("add_route", &TravelPlannerLib::addRoute,
             "Add a route between two cities",
             py::arg("source"), py::arg("destination"), py::arg("distance"))
        .def("delete_route", &TravelPlannerLib::deleteRoute,
             "Delete a route between two cities",
             py::arg("source"), py::arg("destination"))
        .def("find_shortest_path", &TravelPlannerLib::findShortestPath,
             "Find the shortest path between two cities using Dijkstra's algorithm",
             py::arg("start"), py::arg("end"))
        .def("find_fewest_stops", &TravelPlannerLib::findFewestStops,
             "Find the path with fewest stops between two cities using BFS",
             py::arg("start"), py::arg("end"))
        .def("get_reachable_cities", &TravelPlannerLib::getReachableCities,
             "Get all cities reachable from a starting city",
             py::arg("start"))
        .def("get_map_stats", &TravelPlannerLib::getMapStats,
             "Get statistics about the map")
        .def("get_all_cities", &TravelPlannerLib::getAllCities,
             "Get all cities in the map")
        .def("get_all_routes", &TravelPlannerLib::getAllRoutes,
             "Get all routes in the map")
        .def("clear", &TravelPlannerLib::clear,
             "Clear all data from the map");
}
