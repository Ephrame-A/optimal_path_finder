from django.urls import path
from . import views

app_name = 'pathfinding_service'

urlpatterns = [
    path('', views.index, name='index'),
    path('api/add_route', views.add_route, name='add_route'),
    path('api/remove_route', views.remove_route, name='remove_route'),
    path('api/shortest_path', views.shortest_path, name='shortest_path'),
    path('api/longest_path', views.longest_path, name='longest_path'),
    path('api/fewest_stops', views.fewest_stops, name='fewest_stops'),
    path('api/reachable', views.reachable_cities, name='reachable'),
    path('api/multi_city_tour', views.multi_city_tour, name='multi_city_tour'),
    path('api/cheapest_network', views.cheapest_network, name='cheapest_network'),
    path('api/graph', views.get_graph, name='graph'),
    path('api/clear', views.clear_all, name='clear'),
    path('api/load_sample', views.load_sample, name='load_sample'),
]
