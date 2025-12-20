import sys
import os
from typing import Dict, Any, List, Tuple
from django.conf import settings

# Add parent directory to path to import the C++ module
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
try:
    import pathfinding
    CPP_AVAILABLE = True
except ImportError:
    print("Warning: C++ pathfinding module not available. Using fallback.")
    CPP_AVAILABLE = False


class PathfindingService:
    """
    Service layer that integrates with the C++ pathfinding engine.
    Provides a Python interface to the C++ algorithms.
    """
    
    def __init__(self):
        if CPP_AVAILABLE:
            self.engine = pathfinding.TravelPlannerLib()
        else:
            self.engine = None
    
    def sync_with_database(self):
        """Load routes from Django database into C++ engine"""
        if not CPP_AVAILABLE or not self.engine:
            return False
        
        from core.models import Route
        
        # Clear existing data
        self.engine.clear()
        
        # Load all routes from database
        routes = Route.objects.select_related('source', 'destination').all()
        for route in routes:
            self.engine.add_route(
                route.source.name,
                route.destination.name,
                route.distance
            )
        
        return True
    
    def add_route(self, source: str, destination: str, distance: int) -> Dict[str, Any]:
        """Add a route to the pathfinding engine"""
        if not CPP_AVAILABLE or not self.engine:
            return {
                'success': False,
                'message': 'Pathfinding engine not available'
            }
        
        result = self.engine.add_route(source, destination, distance)
        return {
            'success': result.success,
            'message': result.message
        }
    
    def delete_route(self, source: str, destination: str) -> Dict[str, Any]:
        """Delete a route from the pathfinding engine"""
        if not CPP_AVAILABLE or not self.engine:
            return {
                'success': False,
                'message': 'Pathfinding engine not available'
            }
        
        result = self.engine.delete_route(source, destination)
        return {
            'success': result.success,
            'message': result.message
        }
    
    def find_shortest_path(self, start: str, end: str) -> Dict[str, Any]:
        """Find shortest path using Dijkstra's algorithm"""
        if not CPP_AVAILABLE or not self.engine:
            return {
                'success': False,
                'message': 'Pathfinding engine not available'
            }
        
        result = self.engine.find_shortest_path(start, end)
        return {
            'success': result.success,
            'message': result.message,
            'path': result.path,
            'distance': result.distance
        }
    
    def find_fewest_stops(self, start: str, end: str) -> Dict[str, Any]:
        """Find path with fewest stops using BFS"""
        if not CPP_AVAILABLE or not self.engine:
            return {
                'success': False,
                'message': 'Pathfinding engine not available'
            }
        
        result = self.engine.find_fewest_stops(start, end)
        return {
            'success': result.success,
            'message': result.message,
            'path': result.path,
            'stops': result.distance  # BFS returns number of stops
        }
    
    def get_reachable_cities(self, start: str) -> List[str]:
        """Get all cities reachable from a starting city"""
        if not CPP_AVAILABLE or not self.engine:
            return []
        
        return self.engine.get_reachable_cities(start)
    
    def get_map_stats(self) -> Dict[str, int]:
        """Get map statistics"""
        if not CPP_AVAILABLE or not self.engine:
            return {'totalCities': 0, 'totalRoutes': 0}
        
        stats = self.engine.get_map_stats()
        return {
            'totalCities': stats.totalCities,
            'totalRoutes': stats.totalRoutes
        }
    
    def get_all_cities(self) -> List[str]:
        """Get all cities in the map"""
        if not CPP_AVAILABLE or not self.engine:
            return []
        
        return self.engine.get_all_cities()
    
    def get_all_routes(self) -> List[Tuple[str, str, int]]:
        """Get all routes in the map"""
        if not CPP_AVAILABLE or not self.engine:
            return []
        
        return self.engine.get_all_routes()


# Singleton instance
_pathfinding_service = None

def get_pathfinding_service() -> PathfindingService:
    """Get the singleton pathfinding service instance"""
    global _pathfinding_service
    if _pathfinding_service is None:
        _pathfinding_service = PathfindingService()
    return _pathfinding_service
