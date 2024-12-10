// Se connecter au serveur WebSocket
var ws = new WebSocket(`ws://${window.location.hostname}/ws`);

// Vérification d'ouverture de la connexion WebSocket
ws.onopen = function(event) {
    console.log("Connexion avec le serveur WebSocket OK");
};

// Fonction qui sera appelée à la réception des messages du serveur
ws.onmessage = function(event) {
    try {
        let data = JSON.parse(event.data);
        document.getElementById("VALEUR").innerHTML = data.vitesse;
        document.getElementById("SENS").innerHTML = data.sens;
    } catch (error) {
        console.error("Erreur lors du traitement des données reçues:", error);
    }
};

// Fonctions ON/OFF pour contrôler un périphérique connecté
function FonctionON() {
    ws.send("ON");
    console.log("Commande ON envoyée");
}

function FonctionOFF() {
    ws.send("OFF");
    console.log("Commande OFF envoyée");
}