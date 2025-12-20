from rest_framework import serializers
from .models import City, Route, ChatSession, ChatMessage


class CitySerializer(serializers.ModelSerializer):
    class Meta:
        model = City
        fields = ['id', 'name', 'created_at']


class RouteSerializer(serializers.ModelSerializer):
    source_name = serializers.CharField(source='source.name', read_only=True)
    destination_name = serializers.CharField(source='destination.name', read_only=True)
    
    class Meta:
        model = Route
        fields = ['id', 'source', 'destination', 'source_name', 'destination_name', 'distance', 'created_at']


class ChatMessageSerializer(serializers.ModelSerializer):
    class Meta:
        model = ChatMessage
        fields = ['id', 'message', 'is_user', 'timestamp', 'intent']


class ChatSessionSerializer(serializers.ModelSerializer):
    messages = ChatMessageSerializer(many=True, read_only=True)
    
    class Meta:
        model = ChatSession
        fields = ['id', 'session_id', 'created_at', 'updated_at', 'messages']


class ChatRequestSerializer(serializers.Serializer):
    """Serializer for incoming chat messages"""
    session_id = serializers.UUIDField(required=False, allow_null=True)
    message = serializers.CharField(max_length=1000)


class ChatResponseSerializer(serializers.Serializer):
    """Serializer for chat bot responses"""
    session_id = serializers.UUIDField()
    user_message = serializers.CharField()
    bot_response = serializers.CharField()
    intent = serializers.CharField(required=False)
    data = serializers.JSONField(required=False)
