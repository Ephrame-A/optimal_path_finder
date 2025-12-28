from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render
import json
import sys
import os

# Add parent directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../..'))
import pathfinder

# Global pathfinder instance
pf = pathfinder.PathFinder()

def index(request):
    """Render the main pathfinder UI"""
    return render(request, 'pathfinder.html')

@csrf_exempt
def add_route(request):
    """Add a route between two cities"""
    if request.method == 'POST':
        data = json.loads(request.body)
        city1 = data.get('city1')
        city2 = data.get('city2')
        distance = int(data.get('distance'))
        
        result = pf.add_city(city1, city2, distance)
        return JsonResponse({
            'success': result.success,
            'message': result.message
        })
    return JsonResponse({'success': False, 'message': 'Invalid request'})

@csrf_exempt
def remove_route(request):
    """Remove a route between two cities"""
    if request.method == 'POST':
        data = json.loads(request.body)
        city1 = data.get('city1')
        city2 = data.get('city2')
        
        result = pf.remove_city(city1, city2)
        return JsonResponse({
            'success': result.success,
            'message': result.message
        })
    return JsonResponse({'success': False, 'message': 'Invalid request'})

@csrf_exempt
def shortest_path(request):
    """Find shortest path between two cities"""
    if request.method == 'POST':
        data = json.loads(request.body)
        start = data.get('start')
        end = data.get('end')
        
        result = pf.find_shortest_path(start, end)
        return JsonResponse({
            'found': result.found,
            'path': result.path,
            'distance': result.distance,
            'message': result.message
        })
    return JsonResponse({'found': False, 'message': 'Invalid request'})

@csrf_exempt
def longest_path(request):
    """Find longest path between two cities"""
    if request.method == 'POST':
        data = json.loads(request.body)
        start = data.get('start')
        end = data.get('end')
        
        result = pf.find_longest_path(start, end)
        return JsonResponse({
            'found': result.found,
            'path': result.path,
            'distance': result.distance,
            'message': result.message
        })
    return JsonResponse({'found': False, 'message': 'Invalid request'})

@csrf_exempt
def fewest_stops(request):
    """Find path with fewest stops"""
    if request.method == 'POST':
        data = json.loads(request.body)
        start = data.get('start')
        end = data.get('end')
        
        result = pf.find_fewest_stops(start, end)
        return JsonResponse({
            'found': result.found,
            'path': result.path,
            'stops': result.stops,
            'message': result.message
        })
    return JsonResponse({'found': False, 'message': 'Invalid request'})

@csrf_exempt
def reachable_cities(request):
    """Find all reachable cities from a start city"""
    if request.method == 'POST':
        data = json.loads(request.body)
        start = data.get('start')
        
        cities = pf.find_reachable_cities(start)
        return JsonResponse({
            'cities': cities,
            'message': f'Found {len(cities)} reachable cities'
        })
    return JsonResponse({'cities': [], 'message': 'Invalid request'})

@csrf_exempt
def multi_city_tour(request):
    """Plan a multi-city tour"""
    if request.method == 'POST':
        data = json.loads(request.body)
        cities = data.get('cities', [])
        
        result = pf.plan_multi_city_tour(cities)
        return JsonResponse({
            'found': result.found,
            'path': result.path,
            'totalDistance': result.totalDistance,
            'message': result.message
        })
    return JsonResponse({'found': False, 'message': 'Invalid request'})

def cheapest_network(request):
    """Find the cheapest network (MST)"""
    result = pf.find_cheapest_network()
    return JsonResponse({
        'found': result.found,
        'edges': result.edges,
        'totalCost': result.totalCost,
        'message': result.message
    })

def get_graph(request):
    """Get current graph data"""
    cities = pf.get_all_cities()
    routes = pf.get_all_routes()
    
    return JsonResponse({
        'cities': cities,
        'routes': routes
    })

@csrf_exempt
def clear_all(request):
    """Clear all graph data"""
    if request.method == 'POST':
        pf.clear_all()
        return JsonResponse({
            'success': True,
            'message': 'All data cleared'
        })
    return JsonResponse({'success': False, 'message': 'Invalid request'})

@csrf_exempt
def load_sample(request):
    """Load sample graph data"""
    if request.method == 'POST':
        # Clear existing data
        pf.clear_all()
        
        # Add comprehensive sample routes - USA cities with realistic distances
        sample_routes = [
            # East Coast Network
            ('Boston', 'New York', 215),
            ('New York', 'Philadelphia', 95),
            ('Philadelphia', 'Baltimore', 100),
            ('Baltimore', 'Washington', 40),
            ('Washington', 'Richmond', 108),
            ('Richmond', 'Charlotte', 298),
            ('Charlotte', 'Atlanta', 245),
            ('Atlanta', 'Jacksonville', 346),
            ('Jacksonville', 'Miami', 345),
            ('Atlanta', 'Savannah', 250),
            
            # Northeast Connections
            ('Boston', 'Portland', 103),
            ('New York', 'Hartford', 110),
            ('Hartford', 'Boston', 100),
            ('Philadelphia', 'Pittsburgh', 305),
            ('Baltimore', 'Pittsburgh', 247),
            
            # Mid-Atlantic to Midwest
            ('Pittsburgh', 'Cleveland', 134),
            ('Cleveland', 'Detroit', 170),
            ('Cleveland', 'Columbus', 143),
            ('Columbus', 'Cincinnati', 107),
            ('Cincinnati', 'Louisville', 100),
            ('Detroit', 'Chicago', 283),
            ('Chicago', 'Milwaukee', 92),
            ('Milwaukee', 'Minneapolis', 340),
            ('Minneapolis', 'Madison', 270),
            ('Madison', 'Milwaukee', 77),
            
            # Southern Network
            ('Atlanta', 'Birmingham', 147),
            ('Birmingham', 'Memphis', 242),
            ('Memphis', 'Nashville', 210),
            ('Nashville', 'Louisville', 175),
            ('Nashville', 'Knoxville', 180),
            ('Charlotte', 'Raleigh', 165),
            ('Raleigh', 'Richmond', 160),
            
            # Texas Network
            ('Dallas', 'Houston', 239),
            ('Houston', 'Austin', 165),
            ('Austin', 'San Antonio', 80),
            ('Dallas', 'San Antonio', 275),
            ('Dallas', 'Oklahoma City', 206),
            
            # South Central
            ('Memphis', 'Little Rock', 133),
            ('Little Rock', 'Oklahoma City', 342),
            ('Oklahoma City', 'Kansas City', 347),
            ('Kansas City', 'St Louis', 248),
            ('St Louis', 'Chicago', 297),
            ('St Louis', 'Memphis', 284),
            ('St Louis', 'Indianapolis', 242),
            ('Indianapolis', 'Cincinnati', 110),
            ('Indianapolis', 'Chicago', 185),
            
            # Mountain West
            ('Denver', 'Colorado Springs', 70),
            ('Denver', 'Salt Lake City', 525),
            ('Salt Lake City', 'Las Vegas', 420),
            ('Las Vegas', 'Phoenix', 297),
            ('Phoenix', 'Tucson', 116),
            ('Denver', 'Kansas City', 600),
            
            # West Coast
            ('Seattle', 'Portland OR', 173),
            ('Portland OR', 'San Francisco', 635),
            ('San Francisco', 'San Jose', 48),
            ('San Jose', 'Los Angeles', 340),
            ('Los Angeles', 'San Diego', 120),
            ('San Diego', 'Phoenix', 355),
            ('Los Angeles', 'Las Vegas', 270),
            
            # Cross-country connections
            ('Chicago', 'Denver', 1004),
            ('Dallas', 'Denver', 781),
            ('Phoenix', 'Denver', 602),
            ('Seattle', 'Minneapolis', 1641),
            ('San Francisco', 'Seattle', 808),
        ]
        
        for city1, city2, distance in sample_routes:
            pf.add_city(city1, city2, distance)
        
        return JsonResponse({
            'success': True,
            'message': 'Sample data loaded'
        })
    return JsonResponse({'success': False, 'message': 'Invalid request'})
