# Travel Planner Chatbot 🗺️

An intelligent Django-based chatbot that integrates with a C++ pathfinding engine to provide natural language travel route planning.

## Features

✨ **Natural Language Interface** - Chat naturally with the bot to plan your travel routes

🚀 **High-Performance Pathfinding** - C++ implementation with Dijkstra's and BFS algorithms

🎨 **Modern Premium UI** - Beautiful dark mode interface with smooth animations

📊 **Multiple Path Finding Modes**:
- Shortest path (Dijkstra's algorithm) 
- Fewest stops (BFS)
- Reachable cities (DFS)

🔧 **Route Management** - Add, delete, and manage routes through conversation

## Tech Stack

### Backend
- **Django 6.0** - Web framework
- **Django REST Framework** - API endpoints
- **C++ with pybind11** - High-performance pathfinding engine
- **SQLite** - Database

### Frontend
- **HTML5/CSS3** - Modern responsive design
- **Vanilla JavaScript** - No framework dependencies
- **Custom animations** - Smooth, premium UX

## Installation

### Prerequisites
- Python 3.12+
- C++ compiler (g++)
- Virtual environment support

### Setup

1. **Clone and navigate to the project**:
   ```bash
   cd /home/abrhame/projects/path_cpp
   ```

2. **Activate virtual environment**:
   ```bash
   source venv/bin/activate
   ```

3. **Database is already migrated**, but if needed:
   ```bash
   cd travel_chatbot
   python manage.py makemigrations
   python manage.py migrate
   ```

4. **C++ extension is already compiled**, but to rebuild:
   ```bash
   cd /home/abrhame/projects/path_cpp
   python setup.py build_ext --inplace
   ```

## Running the Application

```bash
source venv/bin/activate
cd travel_chatbot
python manage.py runserver
```

Then open your browser to: **http://localhost:8000**

## Usage Examples

### Adding Routes
```
User: Add route from Paris to London 500km
Bot:  ✅ Successfully added: Paris ↔️ London (500km)
```

### Finding Paths
```
User: Find shortest path from Paris to Berlin
Bot:  🗺️ Optimal Route Found!
      📍 Path: Paris → London → Berlin
      📏 Total Distance: 1200km
```

### Exploring Options
```
User: Show reachable cities from Paris
Bot:  🌍 Cities reachable from Paris:
      London, Berlin, Rome, Madrid
```

### Getting Information
```
User: Show map statistics
Bot:  📊 Map Statistics:
      🏙️ Total Cities: 10
      🛣️ Total Routes: 15
```

## API Endpoints

- `POST /api/chat/` - Send a chat message
- `POST /api/sessions/new/` - Create new chat session
- `GET /api/sessions/` - List all sessions
- `GET /api/cities/` - List all cities
- `GET /api/routes/` - List all routes

## Project Structure

```
path_cpp/
├── venv/                          # Virtual environment
├── travel_planner.cpp             # Original C++ CLI application
├── travel_planner_lib.h           # Refactored C++ library
├── pathfinding_wrapper.cpp        # pybind11 Python bindings
├── setup.py                       # C++ build configuration
├── pathfinding.*.so               # Compiled C++ module
├── travel_chatbot/                # Django project
│   ├── core/                      # Core app (models, views, APIs)
│   ├── chatbot/                   # NLP processor
│   ├── pathfinding_service/       # C++ integration service
│   ├── templates/                 # HTML templates
│   │   └── chat.html             # Main chat interface
│   ├── static/                    # Static assets
│   │   ├── css/style.css         # Premium dark mode styles
│   │   └── js/chat.js            # Chat functionality
│   └── travel_chatbot/            # Project settings
└── requirements.txt               # Python dependencies
```

## Architecture

```
┌─────────────┐
│   Browser   │
│   (User)    │
└──────┬──────┘
       │
       ▼
┌─────────────────────┐
│  Django Templates   │
│  + JavaScript       │
└──────┬──────────────┘
       │
       ▼
┌─────────────────────┐
│  Django REST API    │
│  (core/views.py)    │
└──────┬──────────────┘
       │
       ▼
┌─────────────────────┐
│  NLP Processor      │
│  (Intent + Entity)  │
└──────┬──────────────┘
       │
       ▼
┌─────────────────────┐
│ Pathfinding Service │
│ (Python Bridge)     │
└──────┬──────────────┘
       │
       ▼
┌─────────────────────┐
│   C++ Engine        │
│ (Dijkstra/BFS/DFS)  │
└─────────────────────┘
```

## NLP Intent Recognition

The chatbot recognizes the following intents:
- `greeting` - Hello, hi, etc.
- `help` - Get help and examples
- `add_route` - Add a new route
- `delete_route` - Remove a route
- `shortest_path` - Find optimal path (Dijkstra)
- `fewest_stops` - Find path with minimum stops (BFS)
- `reachable_cities` - Show accessible cities (DFS)
- `list_cities` - Show all cities
- `list_routes` - Show all routes
- `map_stats` - Display statistics

## Development

### Testing C++ Module
```python
import pathfinding

planner = pathfinding.TravelPlannerLib()
result = planner.add_route("Paris", "London", 500)
print(result.success, result.message)

path = planner.find_shortest_path("Paris", "London")
print(path.path, path.distance)
```

### Adding New Intents
1. Add pattern to `chatbot/nlp_processor.py`
2. Create handler function in `core/views.py`
3. Update help text

## License

MIT License - Feel free to use and modify!

## Author

Built with ❤️ using Django + C++ integration
# optimal_path_finder
