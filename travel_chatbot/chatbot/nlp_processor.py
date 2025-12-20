import re
from typing import Dict, Any, Optional, List


class NLPProcessor:
    """
    Simple NLP processor for extracting intents and entities from user messages.
    Supports natural language queries for pathfinding operations.
    """
    
    def __init__(self):
        # Define intent patterns (regex patterns and keywords)
        self.intent_patterns = {
            'add_route': [
                r'add\s+(?:a\s+)?route\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
                r'create\s+(?:a\s+)?route\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
                r'connect\s+(\w+)\s+(?:and|to|with)\s+(\w+)',
            ],
            'delete_route': [
                r'delete\s+(?:the\s+)?route\s+(?:from\s+|between\s+)?(\w+)\s+(?:to|and)\s+(\w+)',
                r'remove\s+(?:the\s+)?route\s+(?:from\s+|between\s+)?(\w+)\s+(?:to|and)\s+(\w+)',
            ],
            'shortest_path': [
                r'(?:find|show|get|what(?:\'s| is))\s+(?:the\s+)?shortest\s+(?:path|route|way)\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
                r'shortest\s+(?:route|path)\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
                r'how\s+(?:do i|can i|to)\s+(?:get|go)\s+from\s+(\w+)\s+to\s+(\w+)',
                r'from\s+(\w+)\s+to\s+(\w+)',
            ],
            'fewest_stops': [
                r'(?:find|show|get)\s+(?:the\s+)?(?:path|route)\s+with\s+(?:the\s+)?(?:fewest|minimum|least)\s+stops?\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
                r'fewest\s+stops?\s+(?:from\s+)?(\w+)\s+to\s+(\w+)',
            ],
            'reachable_cities': [
                r'(?:show|list|get|what)\s+(?:cities|places)?\s+(?:reachable|accessible)\s+from\s+(\w+)',
                r'where\s+can\s+i\s+go\s+from\s+(\w+)',
                r'reachable\s+(?:from\s+)?(\w+)',
            ],
            'list_cities': [
                r'(?:show|list|get|what are)\s+(?:all\s+)?(?:the\s+)?cities',
                r'what\s+cities\s+(?:do you have|are available)',
            ],
            'list_routes': [
                r'(?:show|list|get|what are)\s+(?:all\s+)?(?:the\s+)?routes',
                r'what\s+routes\s+(?:do you have|are available)',
            ],
            'map_stats': [
                r'(?:show|get|what are)\s+(?:the\s+)?(?:map\s+)?(?:stats|statistics)',
                r'how\s+many\s+cities',
                r'how\s+many\s+routes',
            ],
            'help': [
                r'\bhelp\b',
                r'what\s+can\s+you\s+do',
                r'commands?',
            ],
            'greeting': [
                r'\b(?:hi|hello|hey|greetings)\b',
            ],
        }
    
    def process(self, message: str) -> Dict[str, Any]:
        """
        Process a user message and extract intent and entities.
        
        Returns:
            dict with 'intent', 'entities', and 'confidence'
        """
        message = message.lower().strip()
        
        # Try to match intent patterns
        for intent, patterns in self.intent_patterns.items():
            for pattern in patterns:
                match = re.search(pattern, message, re.IGNORECASE)
                if match:
                    entities = self._extract_entities(intent, match, message)
                    return {
                        'intent': intent,
                        'entities': entities,
                        'confidence': 0.9
                    }
        
        # No pattern matched
        return {
            'intent': 'unknown',
            'entities': {},
            'confidence': 0.0
        }
    
    def _extract_entities(self, intent: str, match: re.Match, full_message: str) -> Dict[str, Any]:
        """Extract entities based on the matched intent"""
        entities = {}
        
        if intent in ['add_route', 'delete_route']:
            if match.groups():
                entities['source'] = match.group(1).title()
                entities['destination'] = match.group(2).title()
                
                # Extract distance for add_route
                if intent == 'add_route':
                    distance_match = re.search(r'(\d+)\s*(?:km|kilometers?|miles?)?', full_message)
                    if distance_match:
                        entities['distance'] = int(distance_match.group(1))
        
        elif intent in ['shortest_path', 'fewest_stops']:
            if match.groups():
                entities['start'] = match.group(1).title()
                entities['end'] = match.group(2).title()
        
        elif intent == 'reachable_cities':
            if match.groups():
                entities['start'] = match.group(1).title()
        
        return entities
    
    def get_help_text(self) -> str:
        """Return help text with example commands"""
        return """I can help you with travel planning! Here's what I can do:

🗺️ **Route Management:**
• "Add route from Paris to London 500km"
• "Delete route between Paris and London"

🔍 **Path Finding:**
• "Find shortest path from Paris to Berlin"
• "Show fewest stops from NYC to LA"
• "How do I get from Paris to London?"

📊 **Information:**
• "Show reachable cities from Paris"
• "List all cities"
• "List all routes"
• "Show map statistics"

Just ask me in natural language!"""
