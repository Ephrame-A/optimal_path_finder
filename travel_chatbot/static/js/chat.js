// Chat application state
const state = {
    sessionId: null,
    isTyping: false,
};

// DOM elements
const elements = {
    messageInput: document.getElementById('message-input'),
    sendBtn: document.getElementById('send-btn'),
    chatMessages: document.getElementById('chat-messages'),
    typingIndicator: document.getElementById('typing-indicator'),
    sessionDisplay: document.getElementById('session-display'),
    newSessionBtn: document.getElementById('new-session-btn'),
    actionButtons: document.querySelectorAll('.action-btn'),
};

// Initialize the application
function init() {
    setupEventListeners();
    autoResizeTextarea();
}

// Setup event listeners
function setupEventListeners() {
    // Send message on button click
    elements.sendBtn.addEventListener('click', sendMessage);

    // Send message on Enter (but allow Shift+Enter for new line)
    elements.messageInput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && !e.shiftKey) {
            e.preventDefault();
            sendMessage();
        }
    });

    // Auto-resize textarea
    elements.messageInput.addEventListener('input', autoResizeTextarea);

    // New session button
    elements.newSessionBtn.addEventListener('click', createNewSession);

    // Quick action buttons
    elements.actionButtons.forEach(btn => {
        btn.addEventListener('click', () => {
            const query = btn.getAttribute('data-query');
            elements.messageInput.value = query;
            sendMessage();
        });
    });
}

// Auto-resize textarea based on content
function autoResizeTextarea() {
    const textarea = elements.messageInput;
    textarea.style.height = 'auto';
    textarea.style.height = Math.min(textarea.scrollHeight, 150) + 'px';
}

// Send message to the API
async function sendMessage() {
    const message = elements.messageInput.value.trim();
    if (!message || state.isTyping) return;

    // Clear input
    elements.messageInput.value = '';
    autoResizeTextarea();

    // Display user message
    addMessage(message, true);

    // Show typing indicator
    showTypingIndicator();

    try {
        // Send to API
        const response = await fetch('/api/chat/', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                session_id: state.sessionId,
                message: message,
            }),
        });

        if (!response.ok) {
            throw new Error('Failed to send message');
        }

        const data = await response.json();

        // Update session ID if new
        if (!state.sessionId && data.session_id) {
            state.sessionId = data.session_id;
            updateSessionDisplay(data.session_id);
        }

        // Hide typing indicator
        hideTypingIndicator();

        // Display bot response
        addMessage(data.bot_response, false, data.intent);

    } catch (error) {
        console.error('Error sending message:', error);
        hideTypingIndicator();
        addMessage('Sorry, I encountered an error. Please try again.', false, 'error');
    }
}

// Add a message to the chat
function addMessage(text, isUser = false, intent = null) {
    const messageDiv = document.createElement('div');
    messageDiv.className = `message ${isUser ? 'user-message' : 'bot-message'}`;

    const avatar = document.createElement('div');
    avatar.className = 'message-avatar';
    avatar.textContent = isUser ? '👤' : '🤖';

    const content = document.createElement('div');
    content.className = 'message-content';

    const textDiv = document.createElement('div');
    textDiv.className = 'message-text';

    // Format the message (handle markdown-like formatting)
    textDiv.innerHTML = formatMessage(text);

    content.appendChild(textDiv);
    messageDiv.appendChild(avatar);
    messageDiv.appendChild(content);

    elements.chatMessages.appendChild(messageDiv);

    // Scroll to bottom
    elements.chatMessages.scrollTop = elements.chatMessages.scrollHeight;
}

// Format message text (simple markdown-like formatting)
function formatMessage(text) {
    // Replace **bold** with <strong>
    text = text.replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>');

    // Replace line breaks with <br>
    text = text.replace(/\n/g, '<br>');

    // Format bullet points
    text = text.replace(/^• (.+)$/gm, '<li>$1</li>');
    if (text.includes('<li>')) {
        text = text.replace(/(<li>.*<\/li>)/s, '<ul>$1</ul>');
    }

    return text;
}

// Show typing indicator
function showTypingIndicator() {
    state.isTyping = true;
    elements.typingIndicator.classList.add('active');
    elements.sendBtn.disabled = true;
}

// Hide typing indicator
function hideTypingIndicator() {
    state.isTyping = false;
    elements.typingIndicator.classList.remove('active');
    elements.sendBtn.disabled = false;
}

// Update session display
function updateSessionDisplay(sessionId) {
    const shortId = sessionId.substring(0, 8) + '...';
    elements.sessionDisplay.textContent = shortId;
    elements.sessionDisplay.title = sessionId;
}

// Create a new session
async function createNewSession() {
    try {
        const response = await fetch('/api/sessions/new/', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to create new session');
        }

        const data = await response.json();

        // Update session ID
        state.sessionId = data.session_id;
        updateSessionDisplay(data.session_id);

        // Clear chat messages (keep welcome message)
        const messages = elements.chatMessages.querySelectorAll('.message:not(.first-message)');
        messages.forEach(msg => msg.remove());

        // Show success message
        addMessage('✨ New conversation started! How can I help you?', false);

    } catch (error) {
        console.error('Error creating new session:', error);
        addMessage('Failed to create new session. Please try again.', false, 'error');
    }
}

// Initialize on page load
document.addEventListener('DOMContentLoaded', init);
