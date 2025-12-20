from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from django.db import transaction

from .models import ChatSession, ChatMessage, City, Route
from .serializers import (
    ChatRequestSerializer, ChatResponseSerializer,
    ChatSessionSerializer, CitySerializer, RouteSerializer
)
from chatbot.nlp_processor import NLPProcessor
from pathfinding_service.service import get_pathfinding_service


# Initialize NLP and pathfinding
nlp = NLPProcessor()
pf_service = get_pathfinding_service()
print(pf_service)

@api_view(['POST'])
def chat(request):
    """
    Main chat endpoint. Processes user messages and returns bot responses.
    """
    serializer = ChatRequestSerializer(data=request.data)
    if not serializer.is_valid():
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
    message = serializer.validated_data['message']
    session_id = serializer.validated_data.get('session_id')
    
    # Get or create chat session
    if session_id:
        try:
            session = ChatSession.objects.get(session_id=session_id)
        except ChatSession.DoesNotExist:
            session = ChatSession.objects.create()
    else:
        session = ChatSession.objects.create()
    
    # Save user message
    user_msg = ChatMessage.objects.create(
        session=session,
        message=message,
        is_user=True
    )
    
    # Process message with NLP
    nlp_result = nlp.process(message)
    intent = nlp_result['intent']
    entities = nlp_result['entities']
    
    # Update user_msg with intent
    user_msg.intent = intent
    user_msg.extracted_data = entities
    user_msg.save()
    
    # Generate bot response based on intent
    bot_response, response_data = handle_intent(intent, entities)
    
    # Save bot message
    ChatMessage.objects.create(
        session=session,
        message=bot_response,
        is_user=False,
        intent=intent,
        extracted_data=response_data
    )
    
    # Return response
    response_serializer = ChatResponseSerializer({
        'session_id': session.session_id,
        'user_message': message,
        'bot_response': bot_response,
        'intent': intent,
        'data': response_data
    })
    
    return Response(response_serializer.data)


def handle_intent(intent: str, entities: dict) -> tuple:
    """
    Handle different intents and return appropriate responses.
    Returns: (bot_message, response_data)
    """
    
    if intent == 'greeting':
        return ("Hello! 👋 I'm your travel planning assistant. I can help you find routes, "
                "add or remove connections, and explore travel options. How can I help you today?", {})
    
    elif intent == 'help':
        return (nlp.get_help_text(), {})
    
    elif intent == 'add_route':
        return handle_add_route(entities)
    
    elif intent == 'delete_route':
        return handle_delete_route(entities)
    
    elif intent == 'shortest_path':
        return handle_shortest_path(entities)
    
    elif intent == 'fewest_stops':
        return handle_fewest_stops(entities)
    
    elif intent == 'reachable_cities':
        return handle_reachable_cities(entities)
    
    elif intent == 'list_cities':
        return handle_list_cities()
    
    elif intent == 'list_routes':
        return handle_list_routes()
    
    elif intent == 'map_stats':
        return handle_map_stats()
    
    else:
        return ("I'm not sure I understand. Try asking something like:\n"
                "• 'Find shortest path from Paris to London'\n"
                "• 'Add route from NYC to Boston 200km'\n"
                "• 'Show all cities'\n"
                "Or type 'help' for more examples.", {})


@transaction.atomic
def handle_add_route(entities: dict) -> tuple:
    """Handle add route intent"""
    source = entities.get('source')
    dest = entities.get('destination')
    distance = entities.get('distance')
    
    if not source or not dest:
        return ("I need both cities to add a route. Example: 'Add route from Paris to London 500km'", {})
    
    if not distance:
        return (f"How far is it from {source} to {dest}? Please include the distance in km.", {})
    
    # Create cities if they don't exist
    source_city, _ = City.objects.get_or_create(name=source)
    dest_city, _ = City.objects.get_or_create(name=dest)
    
    # Create route (bidirectional)
    try:
        route, created = Route.objects.get_or_create(
            source=source_city if source < dest else dest_city,
            destination=dest_city if source < dest else source_city,
            defaults={'distance': distance}
        )
        
        if not created:
            route.distance = distance
            route.save()
            message = f"✅ Updated route: {source} ↔️ {dest} ({distance}km)"
        else:
            message = f"✅ Successfully added: {source} ↔️ {dest} ({distance}km)"
        
        # Sync with C++ engine
        pf_service.sync_with_database()
        
        return (message, {'source': source, 'destination': dest, 'distance': distance})
    except Exception as e:
        return (f"❌ Error adding route: {str(e)}", {})


@transaction.atomic
def handle_delete_route(entities: dict) -> tuple:
    """Handle delete route intent"""
    source = entities.get('source')
    dest = entities.get('destination')
    
    if not source or not dest:
        return ("I need both cities to delete a route. Example: 'Delete route between Paris and London'", {})
    
    try:
        # Find and delete route
        route = Route.objects.filter(
            source__name__iexact=source, destination__name__iexact=dest
        ).first() or Route.objects.filter(
            source__name__iexact=dest, destination__name__iexact=source
        ).first()
        
        if route:
            route.delete()
            pf_service.sync_with_database()
            return (f"✅ Deleted route between {source} and {dest}", {})
        else:
            return (f"❌ No route found between {source} and {dest}", {})
    except Exception as e:
        return (f"❌ Error deleting route: {str(e)}", {})


def handle_shortest_path(entities: dict) -> tuple:
    """Handle shortest path intent"""
    start = entities.get('start')
    end = entities.get('end')
    
    if not start or not end:
        return ("Where would you like to go? Example: 'Find shortest path from Paris to London'", {})
    
    # Sync database with C++ engine
    pf_service.sync_with_database()
    
    # Find path
    result = pf_service.find_shortest_path(start, end)
    
    if result['success']:
        path_str = " → ".join(result['path'])
        message = f"🗺️ **Optimal Route Found!**\n\n"
        message += f"📍 Path: {path_str}\n"
        message += f"📏 Total Distance: {result['distance']}km"
        return (message, result)
    else:
        return (f"❌ {result['message']}", {})


def handle_fewest_stops(entities: dict) -> tuple:
    """Handle fewest stops intent"""
    start = entities.get('start')
    end = entities.get('end')
    
    if not start or not end:
        return ("Where would you like to go? Example: 'Show route with fewest stops from Paris to London'", {})
    
    pf_service.sync_with_database()
    result = pf_service.find_fewest_stops(start, end)
    
    if result['success']:
        path_str = " → ".join(result['path'])
        message = f"🗺️ **Route with Fewest Stops:**\n\n"
        message += f"📍 Path: {path_str}\n"
        message += f"🚏 Number of Stops: {result['stops']}"
        return (message, result)
    else:
        return (f"❌ {result['message']}", {})


def handle_reachable_cities(entities: dict) -> tuple:
    """Handle reachable cities intent"""
    start = entities.get('start')
    
    if not start:
        return ("From which city? Example: 'Show reachable cities from Paris'", {})
    
    pf_service.sync_with_database()
    cities = pf_service.get_reachable_cities(start)
    
    if cities:
        cities_str = ", ".join(cities)
        message = f"🌍 **Cities reachable from {start}:**\n\n{cities_str}"
        return (message, {'start': start, 'reachable': cities})
    else:
        return (f"No cities found reachable from {start}", {})


def handle_list_cities() -> tuple:
    """Handle list cities intent"""
    cities = City.objects.all().order_by('name')
    
    if cities:
        city_names = [city.name for city in cities]
        cities_str = ", ".join(city_names)
        message = f"🏙️ **All Cities ({len(city_names)}):**\n\n{cities_str}"
        return (message, {'cities': city_names})
    else:
        return ("No cities in the database yet. Add some routes to get started!", {})


def handle_list_routes() -> tuple:
    """Handle list routes intent"""
    routes = Route.objects.select_related('source', 'destination').all()
    
    if routes:
        route_list = [f"{r.source.name} ↔️ {r.destination.name} ({r.distance}km)" for r in routes]
        routes_str = "\n".join(route_list)
        message = f"🛣️ **All Routes ({len(route_list)}):**\n\n{routes_str}"
        return (message, {'routes': route_list})
    else:
        return ("No routes in the database yet. Add some routes to get started!", {})


def handle_map_stats() -> tuple:
    """Handle map stats intent"""
    pf_service.sync_with_database()
    stats = pf_service.get_map_stats()
    
    message = f"📊 **Map Statistics:**\n\n"
    message += f"🏙️ Total Cities: {stats['totalCities']}\n"
    message += f"🛣️ Total Routes: {stats['totalRoutes']}"
    
    return (message, stats)


# Additional REST endpoints

@api_view(['GET'])
def list_sessions(request):
    """List all chat sessions"""
    sessions = ChatSession.objects.all()
    serializer = ChatSessionSerializer(sessions, many=True)
    return Response(serializer.data)


@api_view(['POST'])
def new_session(request):
    """Create a new chat session"""
    session = ChatSession.objects.create()
    serializer = ChatSessionSerializer(session)
    return Response(serializer.data)


@api_view(['GET'])
def get_session(request, session_id):
    """Get a specific chat session with messages"""
    try:
        session = ChatSession.objects.get(session_id=session_id)
        serializer = ChatSessionSerializer(session)
        return Response(serializer.data)
    except ChatSession.DoesNotExist:
        return Response({'error': 'Session not found'}, status=status.HTTP_404_NOT_FOUND)


@api_view(['GET'])
def list_cities(request):
    """List all cities"""
    cities = City.objects.all().order_by('name')
    serializer = CitySerializer(cities, many=True)
    return Response(serializer.data)


@api_view(['GET'])
def list_routes_api(request):
    """List all routes"""
    routes = Route.objects.select_related('source', 'destination').all()
    serializer = RouteSerializer(routes, many=True)
    return Response(serializer.data)
