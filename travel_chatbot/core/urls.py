from django.urls import path
from . import views

urlpatterns = [
    # Main chat endpoint
    path('chat/', views.chat, name='chat'),
    
    # Session management
    path('sessions/', views.list_sessions, name='list_sessions'),
    path('sessions/new/', views.new_session, name='new_session'),
    path('sessions/<uuid:session_id>/', views.get_session, name='get_session'),
    
    # Data endpoints
    path('cities/', views.list_cities, name='list_cities'),
    path('routes/', views.list_routes_api, name='list_routes'),
]
