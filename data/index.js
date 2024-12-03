let socket;

function setupWebSocket() {
    // Connexion WebSocket à l'ESP32
    socket = new WebSocket('ws://' + window.location.hostname + '/ws');
    
    socket.onopen = function(event) {
        console.log("Connexion WebSocket réussie");
    };

    socket.onmessage = function(event) {
        // Lorsque la vitesse est reçue depuis l'ESP32, mettez à jour l'affichage
        const vitesse = event.data;
        document.getElementById("vitesse").innerText = vitesse;
    };

    socket.onerror = function(error) {
        console.log("Erreur WebSocket: " + error);
    };

    socket.onclose = function(event) {
        console.log("Connexion WebSocket fermée");
    };
}

// Fonction pour allumer la LED via WebSocket
function FonctionON() {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send("ON");
    }
}

// Fonction pour éteindre la LED via WebSocket
function FonctionOFF() {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send("OFF");
    }
}

// Initialiser la connexion WebSocket
setupWebSocket();