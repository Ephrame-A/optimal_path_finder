class GraphVisualizer {
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.nodes = new Map();
        this.edges = [];
        this.offsetX = 0;
        this.offsetY = 0;
        this.scale = 1;
        this.isDragging = false;
        this.draggedNode = null;
        this.dragStart = { x: 0, y: 0 };
        this.highlightedPath = [];
        this.highlightedNodes = new Set();
        this.visitedNodes = new Set(); // Nodes that icon has reached
        this.animationFrame = null;
        this.movingIcon = null; // {type: 'bus'|'person', position: 0-1, path: [...]}

        this.init();
    }

    init() {
        this.resizeCanvas();
        window.addEventListener('resize', () => this.resizeCanvas());

        // Mouse events
        this.canvas.addEventListener('mousedown', (e) => this.handleMouseDown(e));
        this.canvas.addEventListener('mousemove', (e) => this.handleMouseMove(e));
        this.canvas.addEventListener('mouseup', () => this.handleMouseUp());
        this.canvas.addEventListener('mouseleave', () => this.handleMouseUp());

        // Wheel for zoom
        this.canvas.addEventListener('wheel', (e) => this.handleWheel(e));

        this.draw();
    }

    resizeCanvas() {
        const rect = this.canvas.getBoundingClientRect();
        this.canvas.width = rect.width;
        this.canvas.height = rect.height;
        this.draw();
    }

    getMousePos(e) {
        const rect = this.canvas.getBoundingClientRect();
        return {
            x: (e.clientX - rect.left - this.offsetX) / this.scale,
            y: (e.clientY - rect.top - this.offsetY) / this.scale
        };
    }

    getNodeAtPos(x, y) {
        for (let [name, node] of this.nodes) {
            const dx = node.x - x;
            const dy = node.y - y;
            const dist = Math.sqrt(dx * dx + dy * dy);
            if (dist < 30) return node;
        }
        return null;
    }

    handleMouseDown(e) {
        const pos = this.getMousePos(e);
        const node = this.getNodeAtPos(pos.x, pos.y);

        if (node) {
            this.draggedNode = node;
            this.dragStart = { x: pos.x - node.x, y: pos.y - node.y };
            this.canvas.style.cursor = 'grabbing';
        } else {
            this.isDragging = true;
            this.dragStart = { x: e.clientX - this.offsetX, y: e.clientY - this.offsetY };
        }
    }

    handleMouseMove(e) {
        const pos = this.getMousePos(e);

        if (this.draggedNode) {
            this.draggedNode.x = pos.x - this.dragStart.x;
            this.draggedNode.y = pos.y - this.dragStart.y;
            this.draw();
        } else if (this.isDragging) {
            this.offsetX = e.clientX - this.dragStart.x;
            this.offsetY = e.clientY - this.dragStart.y;
            this.draw();
        } else {
            // Show cursor feedback
            const node = this.getNodeAtPos(pos.x, pos.y);
            this.canvas.style.cursor = node ? 'grab' : 'default';
        }
    }

    handleMouseUp() {
        this.isDragging = false;
        this.draggedNode = null;
        this.canvas.style.cursor = 'default';
    }

    handleWheel(e) {
        e.preventDefault();
        const delta = e.deltaY > 0 ? 0.9 : 1.1;
        this.scale *= delta;
        this.scale = Math.max(0.5, Math.min(3, this.scale));
        this.draw();
    }

    addNode(name, x, y) {
        if (!this.nodes.has(name)) {
            // Assign random color to each node
            const colors = ['#ef4444', '#f59e0b', '#10b981', '#3b82f6', '#8b5cf6', '#ec4899', '#06b6d4', '#84cc16'];
            const color = colors[Math.abs(name.split('').reduce((acc, c) => acc + c.charCodeAt(0), 0)) % colors.length];
            this.nodes.set(name, { x, y, name, color });
        }
    }

    addEdge(from, to, weight) {
        this.edges = this.edges.filter(e =>
            !(e.from === from && e.to === to) && !(e.from === to && e.to === from)
        );
        this.edges.push({ from, to, weight });
    }

    removeEdge(from, to) {
        this.edges = this.edges.filter(e =>
            !(e.from === from && e.to === to) && !(e.from === to && e.to === from)
        );
    }

    autoLayout() {
        const nodeCount = this.nodes.size;
        if (nodeCount === 0) return;

        // Random positioning with padding from edges
        const padding = 80;
        const width = this.canvas.width - 2 * padding;
        const height = this.canvas.height - 2 * padding;

        for (let [name, node] of this.nodes) {
            // Random position within bounds
            node.x = padding + Math.random() * width;
            node.y = padding + Math.random() * height;
        }

        this.offsetX = 0;
        this.offsetY = 0;
        this.scale = 1;
    }

    setHighlightPath(path, iconType = 'bus') {
        this.highlightedPath = path;
        this.highlightedNodes = new Set(path);
        this.visitedNodes = new Set(); // Reset visited nodes
        if (path.length > 0) {
            this.visitedNodes.add(path[0]); // Mark start as visited
        }
        this.movingIcon = { type: iconType, position: 0, path: path };
        this.startAnimation();
    }

    setHighlightNodes(nodes) {
        this.highlightedNodes = new Set(nodes);
        this.highlightedPath = [];
        this.visitedNodes.clear();
        this.movingIcon = null;
        this.startAnimation();
    }

    clearHighlights() {
        this.highlightedPath = [];
        this.highlightedNodes.clear();
        this.visitedNodes.clear();
        this.movingIcon = null;
        this.stopAnimation();
        this.draw();
    }

    startAnimation() {
        if (this.animationFrame) return;

        let frame = 0;
        const animate = () => {
            this.draw(frame);
            frame++;

            // Update moving icon position
            if (this.movingIcon && this.movingIcon.path.length > 1) {
                // Different speeds for bus vs person (both slower for better visibility)
                const speed = this.movingIcon.type === 'person' ? 0.0008 : 0.0015;
                this.movingIcon.position += speed;

                // Mark nodes as visited when icon reaches them
                const path = this.movingIcon.path;
                const totalSegments = path.length - 1;
                const currentSegment = Math.floor(this.movingIcon.position * totalSegments);
                if (currentSegment < path.length) {
                    this.visitedNodes.add(path[currentSegment]);
                    if (currentSegment + 1 < path.length) {
                        // Check if we're close to next node
                        const segmentProgress = (this.movingIcon.position * totalSegments) - currentSegment;
                        if (segmentProgress > 0.9) {
                            this.visitedNodes.add(path[currentSegment + 1]);
                        }
                    }
                }

                if (this.movingIcon.position >= 1) {
                    this.movingIcon.position = 1;
                    // Mark final destination as visited
                    if (path.length > 0) {
                        this.visitedNodes.add(path[path.length - 1]);
                    }
                    this.stopAnimation();
                }
            }

            this.animationFrame = requestAnimationFrame(animate);
        };
        animate();
    }

    stopAnimation() {
        if (this.animationFrame) {
            cancelAnimationFrame(this.animationFrame);
            this.animationFrame = null;
        }
    }

    getPositionOnPath(progress) {
        if (!this.movingIcon || this.movingIcon.path.length < 2) return null;

        const path = this.movingIcon.path;
        const totalSegments = path.length - 1;
        const segmentProgress = progress * totalSegments;
        const segmentIndex = Math.floor(segmentProgress);
        const segmentFraction = segmentProgress - segmentIndex;

        if (segmentIndex >= totalSegments) return null;

        const fromNode = this.nodes.get(path[segmentIndex]);
        const toNode = this.nodes.get(path[segmentIndex + 1]);

        if (!fromNode || !toNode) return null;

        return {
            x: fromNode.x + (toNode.x - fromNode.x) * segmentFraction,
            y: fromNode.y + (toNode.y - fromNode.y) * segmentFraction,
            angle: Math.atan2(toNode.y - fromNode.y, toNode.x - fromNode.x)
        };
    }

    draw(frame = 0) {
        const ctx = this.ctx;
        ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        ctx.save();
        ctx.translate(this.offsetX, this.offsetY);
        ctx.scale(this.scale, this.scale);

        // Draw edges
        this.drawEdges(ctx, frame);

        // Draw nodes
        this.drawNodes(ctx, frame);

        // Draw moving icon
        this.drawMovingIcon(ctx);

        ctx.restore();
    }

    drawEdges(ctx, frame) {
        for (let edge of this.edges) {
            const fromNode = this.nodes.get(edge.from);
            const toNode = this.nodes.get(edge.to);

            if (!fromNode || !toNode) continue;

            const isHighlighted = this.isEdgeHighlighted(edge.from, edge.to);

            ctx.beginPath();
            ctx.moveTo(fromNode.x, fromNode.y);
            ctx.lineTo(toNode.x, toNode.y);

            if (isHighlighted) {
                const pulse = Math.sin(frame * 0.1) * 0.5 + 0.5;
                ctx.strokeStyle = `rgba(99, 102, 241, ${0.7 + pulse * 0.3})`;
                ctx.lineWidth = 4 + pulse * 2;
            } else {
                ctx.strokeStyle = '#cbd5e1';
                ctx.lineWidth = 2;
            }

            ctx.stroke();

            // Weight label
            const midX = (fromNode.x + toNode.x) / 2;
            const midY = (fromNode.y + toNode.y) / 2;

            ctx.fillStyle = isHighlighted ? '#6366f1' : '#64748b';
            ctx.font = 'bold 14px Inter';
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';

            const metrics = ctx.measureText(edge.weight);
            ctx.fillStyle = 'white';
            ctx.fillRect(midX - metrics.width / 2 - 4, midY - 8, metrics.width + 8, 16);

            ctx.fillStyle = isHighlighted ? '#6366f1' : '#64748b';
            ctx.fillText(edge.weight, midX, midY);
        }
    }

    drawNodes(ctx, frame) {
        for (let [name, node] of this.nodes) {
            const isHighlighted = this.highlightedNodes.has(name);
            const isVisited = this.visitedNodes.has(name);

            ctx.beginPath();
            ctx.arc(node.x, node.y, 30, 0, 2 * Math.PI);

            if (isHighlighted) {
                const pulse = Math.sin(frame * 0.1) * 0.5 + 0.5;

                if (isVisited) {
                    // Visited nodes get a bright green glow
                    const gradient = ctx.createRadialGradient(node.x, node.y, 0, node.x, node.y, 30 + pulse * 10);
                    gradient.addColorStop(0, '#10b981');
                    gradient.addColorStop(1, '#059669');
                    ctx.fillStyle = gradient;
                    ctx.shadowColor = 'rgba(16, 185, 129, 0.6)';
                    ctx.shadowBlur = 20 + pulse * 10;
                } else {
                    // Non-visited highlighted nodes
                    const gradient = ctx.createRadialGradient(node.x, node.y, 0, node.x, node.y, 30 + pulse * 10);
                    gradient.addColorStop(0, '#8b5cf6');
                    gradient.addColorStop(1, '#6366f1');
                    ctx.fillStyle = gradient;
                    ctx.shadowColor = 'rgba(99, 102, 241, 0.5)';
                    ctx.shadowBlur = 15 + pulse * 10;
                }
            } else {
                // Use node's assigned color
                ctx.fillStyle = node.color || '#f1f5f9';
                ctx.shadowBlur = 0;
            }

            ctx.fill();
            ctx.strokeStyle = isVisited ? '#10b981' : (isHighlighted ? '#6366f1' : '#cbd5e1');
            ctx.lineWidth = isVisited ? 4 : 3;
            ctx.stroke();

            ctx.shadowBlur = 0;
            ctx.fillStyle = (isHighlighted || isVisited) ? '#ffffff' : '#1e293b';
            ctx.font = 'bold 11px Inter';
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';
            ctx.fillText(name, node.x, node.y);
        }
    }

    drawMovingIcon(ctx) {
        if (!this.movingIcon) return;

        const pos = this.getPositionOnPath(this.movingIcon.position);
        if (!pos) return;

        ctx.save();
        ctx.translate(pos.x, pos.y);
        ctx.rotate(pos.angle);

        if (this.movingIcon.type === 'bus') {
            this.drawBus(ctx);
        } else if (this.movingIcon.type === 'person') {
            this.drawPerson(ctx);
        }

        ctx.restore();
    }

    drawBus(ctx) {
        // Draw a simple bus icon
        ctx.fillStyle = '#fbbf24';
        ctx.fillRect(-20, -12, 40, 24);

        // Windows
        ctx.fillStyle = '#93c5fd';
        ctx.fillRect(-15, -8, 10, 8);
        ctx.fillRect(5, -8, 10, 8);

        // Wheels
        ctx.fillStyle = '#1e293b';
        ctx.beginPath();
        ctx.arc(-10, 12, 5, 0, 2 * Math.PI);
        ctx.arc(10, 12, 5, 0, 2 * Math.PI);
        ctx.fill();

        // Front
        ctx.fillStyle = '#ef4444';
        ctx.fillRect(18, -5, 4, 10);
    }

    drawPerson(ctx) {
        // Draw a walking person icon
        const walkCycle = Math.sin(this.movingIcon.position * 20) * 0.3;

        // Head
        ctx.fillStyle = '#f59e0b';
        ctx.beginPath();
        ctx.arc(0, -15, 6, 0, 2 * Math.PI);
        ctx.fill();

        // Body
        ctx.strokeStyle = '#f59e0b';
        ctx.lineWidth = 3;
        ctx.lineCap = 'round';

        // Torso
        ctx.beginPath();
        ctx.moveTo(0, -9);
        ctx.lineTo(0, 5);
        ctx.stroke();

        // Arms
        ctx.beginPath();
        ctx.moveTo(0, -5);
        ctx.lineTo(-8, 2 + walkCycle * 5);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, -5);
        ctx.lineTo(8, 2 - walkCycle * 5);
        ctx.stroke();

        // Legs
        ctx.beginPath();
        ctx.moveTo(0, 5);
        ctx.lineTo(-6, 15 - walkCycle * 8);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, 5);
        ctx.lineTo(6, 15 + walkCycle * 8);
        ctx.stroke();
    }

    isEdgeHighlighted(from, to) {
        if (this.highlightedPath.length < 2) return false;

        for (let i = 0; i < this.highlightedPath.length - 1; i++) {
            const a = this.highlightedPath[i];
            const b = this.highlightedPath[i + 1];
            if ((a === from && b === to) || (a === to && b === from)) {
                return true;
            }
        }
        return false;
    }

    clear() {
        this.nodes.clear();
        this.edges = [];
        this.clearHighlights();
        this.offsetX = 0;
        this.offsetY = 0;
        this.scale = 1;
        this.draw();
    }
}

// API Handler
class PathFinderAPI {
    constructor(baseUrl = '/api') {
        this.baseUrl = baseUrl;
    }

    async addRoute(city1, city2, distance) {
        const response = await fetch(`${this.baseUrl}/add_route`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ city1, city2, distance })
        });
        return await response.json();
    }

    async removeRoute(city1, city2) {
        const response = await fetch(`${this.baseUrl}/remove_route`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ city1, city2 })
        });
        return await response.json();
    }

    async findShortestPath(start, end) {
        const response = await fetch(`${this.baseUrl}/shortest_path`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ start, end })
        });
        return await response.json();
    }

    async findLongestPath(start, end) {
        const response = await fetch(`${this.baseUrl}/longest_path`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ start, end })
        });
        return await response.json();
    }

    async findFewestStops(start, end) {
        const response = await fetch(`${this.baseUrl}/fewest_stops`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ start, end })
        });
        return await response.json();
    }

    async findReachable(start) {
        const response = await fetch(`${this.baseUrl}/reachable`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ start })
        });
        return await response.json();
    }

    async planTour(cities) {
        const response = await fetch(`${this.baseUrl}/multi_city_tour`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ cities })
        });
        return await response.json();
    }

    async findCheapestNetwork() {
        const response = await fetch(`${this.baseUrl}/cheapest_network`, {
            method: 'GET'
        });
        return await response.json();
    }

    async getGraph() {
        const response = await fetch(`${this.baseUrl}/graph`, { method: 'GET' });
        return await response.json();
    }

    async clearAll() {
        const response = await fetch(`${this.baseUrl}/clear`, { method: 'POST' });
        return await response.json();
    }

    async loadSample() {
        const response = await fetch(`${this.baseUrl}/load_sample`, { method: 'POST' });
        return await response.json();
    }
}

// App Controller
class PathFinderApp {
    constructor() {
        this.visualizer = new GraphVisualizer('graphCanvas');
        this.api = new PathFinderAPI();
        this.initEventListeners();
        this.loadGraph();
    }

    initEventListeners() {
        // Build map buttons
        document.getElementById('addRouteBtn').addEventListener('click', () => this.addRoute());
        document.getElementById('removeRouteBtn').addEventListener('click', () => this.removeRoute());

        // Feature buttons
        document.getElementById('shortestPathBtn').addEventListener('click', () => this.shortestPath());
        document.getElementById('longestPathBtn').addEventListener('click', () => this.longestPath());
        document.getElementById('fewestStopsBtn').addEventListener('click', () => this.fewestStops());
        document.getElementById('reachableBtn').addEventListener('click', () => this.reachable());
        document.getElementById('multiCityBtn').addEventListener('click', () => this.multiCityTour());
        document.getElementById('cheapestNetworkBtn').addEventListener('click', () => this.cheapestNetwork());

        // Utility buttons
        document.getElementById('clearAllBtn').addEventListener('click', () => this.clearAll());
        document.getElementById('loadSampleBtn').addEventListener('click', () => this.loadSample());

        // Result panel
        document.getElementById('closeResultBtn').addEventListener('click', () => this.hideResult());

        // Modal
        document.getElementById('modalCancelBtn').addEventListener('click', () => this.hideModal());
    }

    async loadGraph() {
        try {
            const data = await this.api.getGraph();
            this.updateVisualization(data);
        } catch (error) {
            console.error('Error loading graph:', error);
        }
    }

    updateVisualization(data) {
        this.visualizer.clear();

        // Add edges and nodes
        if (data.routes) {
            for (let [city1, city2, distance] of data.routes) {
                this.visualizer.addNode(city1, 0, 0);
                this.visualizer.addNode(city2, 0, 0);
                this.visualizer.addEdge(city1, city2, distance);
            }
        }

        this.visualizer.autoLayout();
        this.visualizer.draw();

        // Update stats
        document.getElementById('cityCount').textContent = data.cities ? data.cities.length : 0;
        document.getElementById('routeCount').textContent = data.routes ? data.routes.length : 0;
    }

    async addRoute() {
        const city1 = document.getElementById('fromCity').value.trim();
        const city2 = document.getElementById('toCity').value.trim();
        const distance = parseInt(document.getElementById('distance').value);

        if (!city1 || !city2 || !distance || distance <= 0) {
            alert('Please enter valid cities and distance');
            return;
        }

        try {
            const result = await this.api.addRoute(city1, city2, distance);
            if (result.success) {
                await this.loadGraph();
                document.getElementById('fromCity').value = '';
                document.getElementById('toCity').value = '';
                document.getElementById('distance').value = '';
                this.showResult('Route Added', `<p>${result.message}</p>`);
            } else {
                alert(result.message);
            }
        } catch (error) {
            console.error('Error:', error);
            alert('Failed to add route');
        }
    }

    async removeRoute() {
        const city1 = document.getElementById('fromCity').value.trim();
        const city2 = document.getElementById('toCity').value.trim();

        if (!city1 || !city2) {
            alert('Please enter both cities');
            return;
        }

        try {
            const result = await this.api.removeRoute(city1, city2);
            if (result.success) {
                await this.loadGraph();
                this.showResult('Route Removed', `<p>${result.message}</p>`);
            } else {
                alert(result.message);
            }
        } catch (error) {
            console.error('Error:', error);
            alert('Failed to remove route');
        }
    }

    async shortestPath() {
        this.showInputModal('Shortest Path', ['Start City', 'End City'], async (values) => {
            try {
                const result = await this.api.findShortestPath(values[0], values[1]);
                if (result.found) {
                    this.visualizer.setHighlightPath(result.path);
                    const pathHtml = `
                        <div class="path-result">
                            <p><strong>Path:</strong> ${result.path.join(' → ')}</p>
                            <span class="distance-badge">${result.distance} km</span>
                        </div>
                    `;
                    this.showResult('Shortest Path', pathHtml);
                } else {
                    alert(result.message);
                }
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to find path');
            }
        });
    }

    async longestPath() {
        this.showInputModal('Longest Path', ['Start City', 'End City'], async (values) => {
            try {
                const result = await this.api.findLongestPath(values[0], values[1]);
                if (result.found) {
                    this.visualizer.setHighlightPath(result.path);
                    const pathHtml = `
                        <div class="path-result">
                            <p><strong>Longest Path:</strong> ${result.path.join(' → ')}</p>
                            <span class="distance-badge" style="background: linear-gradient(135deg, #ef4444, #dc2626);">${result.distance} km</span>
                        </div>
                    `;
                    this.showResult('Longest Path', pathHtml);
                } else {
                    alert(result.message);
                }
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to find path');
            }
        });
    }

    async fewestStops() {
        this.showInputModal('Fewest Stops', ['Start City', 'End City'], async (values) => {
            try {
                const result = await this.api.findFewestStops(values[0], values[1]);
                if (result.found) {
                    this.visualizer.setHighlightPath(result.path);
                    const pathHtml = `
                        <div class="path-result">
                            <p><strong>Path:</strong> ${result.path.join(' → ')}</p>
                            <span class="distance-badge">${result.stops} stops</span>
                        </div>
                    `;
                    this.showResult('Fewest Stops', pathHtml);
                } else {
                    alert(result.message);
                }
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to find path');
            }
        });
    }

    async reachable() {
        this.showInputModal('Reachable Cities', ['Start City'], async (values) => {
            try {
                const result = await this.api.findReachable(values[0]);
                if (result.cities && result.cities.length > 0) {
                    this.visualizer.setHighlightNodes([values[0], ...result.cities]);
                    const citiesHtml = `
                        <div class="city-list">
                            ${result.cities.map(city => `<span class="city-badge">${city}</span>`).join('')}
                        </div>
                    `;
                    this.showResult('Reachable Cities', citiesHtml);
                } else {
                    alert('No reachable cities found');
                }
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to find reachable cities');
            }
        });
    }

    async multiCityTour() {
        this.showInputModal('Multi-City Tour', ['Cities (comma-separated)'], async (values) => {
            const cities = values[0].split(',').map(c => c.trim()).filter(c => c);
            if (cities.length < 2) {
                alert('Please enter at least 2 cities');
                return;
            }

            try {
                const result = await this.api.planTour(cities);
                if (result.found) {
                    this.visualizer.setHighlightPath(result.path, 'person');
                    const pathHtml = `
                        <div class="path-result">
                            <p><strong>Tour:</strong> ${result.path.join(' → ')}</p>
                            <span class="distance-badge">${result.totalDistance} km</span>
                        </div>
                    `;
                    this.showResult('Multi-City Tour', pathHtml);
                } else {
                    alert(result.message);
                }
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to plan tour');
            }
        });
    }

    async cheapestNetwork() {
        try {
            const result = await this.api.findCheapestNetwork();
            if (result.found) {
                // Highlight MST edges
                const nodes = new Set();
                result.edges.forEach(([a, b]) => {
                    nodes.add(a);
                    nodes.add(b);
                });
                this.visualizer.setHighlightNodes(Array.from(nodes));

                const edgesHtml = `
                    <div class="edge-list">
                        ${result.edges.map(([a, b, w]) => `
                            <div class="edge-item">
                                <span class="edge-cities">${a} ↔ ${b}</span>
                                <span class="edge-weight">${w} km</span>
                            </div>
                        `).join('')}
                        <div class="edge-item" style="background: linear-gradient(135deg, #6366f1, #8b5cf6); color: white; font-weight: 600;">
                            <span>Total Cost</span>
                            <span>${result.totalCost} km</span>
                        </div>
                    </div>
                `;
                this.showResult('Cheapest Network (MST)', edgesHtml);
            } else {
                alert(result.message);
            }
        } catch (error) {
            console.error('Error:', error);
            alert('Failed to find cheapest network');
        }
    }

    async clearAll() {
        if (confirm('Are you sure you want to clear all data?')) {
            try {
                await this.api.clearAll();
                await this.loadGraph();
                this.hideResult();
            } catch (error) {
                console.error('Error:', error);
                alert('Failed to clear data');
            }
        }
    }

    async loadSample() {
        try {
            await this.api.loadSample();
            await this.loadGraph();
            this.showResult('Sample Loaded', '<p>Sample graph has been loaded successfully!</p>');
        } catch (error) {
            console.error('Error:', error);
            alert('Failed to load sample');
        }
    }

    showInputModal(title, fields, callback) {
        const modal = document.getElementById('inputModal');
        const modalTitle = document.getElementById('modalTitle');
        const modalBody = document.getElementById('modalBody');
        const okBtn = document.getElementById('modalOkBtn');

        modalTitle.textContent = title;
        modalBody.innerHTML = fields.map((field, i) => `
            <div class="form-group">
                <label>${field}</label>
                <input type="text" id="modalInput${i}" placeholder="${field}">
            </div>
        `).join('');

        modal.classList.add('active');

        const handleOk = async () => {
            const values = fields.map((_, i) =>
                document.getElementById(`modalInput${i}`).value.trim()
            );

            if (values.some(v => !v)) {
                alert('Please fill all fields');
                return;
            }

            modal.classList.remove('active');
            okBtn.removeEventListener('click', handleOk);
            await callback(values);
        };

        okBtn.addEventListener('click', handleOk);
    }

    hideModal() {
        document.getElementById('inputModal').classList.remove('active');
    }

    showResult(title, content) {
        document.getElementById('resultTitle').textContent = title;
        document.getElementById('resultContent').innerHTML = content;
        document.getElementById('resultPanel').classList.add('active');
    }

    hideResult() {
        document.getElementById('resultPanel').classList.remove('active');
        this.visualizer.clearHighlights();
    }
}

// Initialize app when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new PathFinderApp();
});
