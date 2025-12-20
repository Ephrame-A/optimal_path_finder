from django.db import models
import uuid


class City(models.Model):
    """Represents a city/location in the travel network"""
    name = models.CharField(max_length=100, unique=True)
    created_at = models.DateTimeField(auto_now_add=True)

    class Meta:
        verbose_name_plural = "Cities"
        ordering = ['name']

    def __str__(self):
        return self.name


class Route(models.Model):
    """Represents a bidirectional route between two cities"""
    source = models.ForeignKey(City, on_delete=models.CASCADE, related_name='routes_from')
    destination = models.ForeignKey(City, on_delete=models.CASCADE, related_name='routes_to')
    distance = models.IntegerField(help_text="Distance in kilometers")
    created_at = models.DateTimeField(auto_now_add=True)

    class Meta:
        unique_together = [['source', 'destination']]
        ordering = ['source__name', 'destination__name']

    def __str__(self):
        return f"{self.source.name} <-> {self.destination.name} ({self.distance}km)"
    
    def save(self, *args, **kwargs):
        # Ensure source < destination alphabetically for consistency
        if self.source.name > self.destination.name:
            self.source, self.destination = self.destination, self.source
        super().save(*args, **kwargs)


class ChatSession(models.Model):
    """Represents a chat conversation session"""
    session_id = models.UUIDField(default=uuid.uuid4, unique=True, editable=False)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    class Meta:
        ordering = ['-created_at']

    def __str__(self):
        return f"Session {self.session_id} - {self.created_at.strftime('%Y-%m-%d %H:%M')}"


class ChatMessage(models.Model):
    """Represents a single message in a chat session"""
    session = models.ForeignKey(ChatSession, on_delete=models.CASCADE, related_name='messages')
    message = models.TextField()
    is_user = models.BooleanField(default=True, help_text="True if message is from user, False if from bot")
    timestamp = models.DateTimeField(auto_now_add=True)
    
    # Optional fields for storing query metadata
    intent = models.CharField(max_length=50, blank=True, null=True)
    extracted_data = models.JSONField(blank=True, null=True)

    class Meta:
        ordering = ['timestamp']

    def __str__(self):
        sender = "User" if self.is_user else "Bot"
        return f"[{sender}] {self.message[:50]}..."
