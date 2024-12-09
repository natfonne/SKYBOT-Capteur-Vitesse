// Se connecter au serveur WebSocket
var ws = new WebSocket(`ws://${window.location.hostname}/ws`);

// Détecter l'ouverture d'une connexion avec le serveur
ws.onopen = function(event) {
    ws.send("Connection avec le serveur Etablie");
    console.log("Connexion avec le serveur OK");
};

// Recevoir les données
ws.onmessage = function(event) {
    let data = JSON.parse(event.data);
    document.getElementById("VALEUR").innerHTML = data.vitesse;
    document.getElementById("SENS").innerHTML = data.sens;
};

function FonctionON() {
    ws.send("ON");
    console.log("ON");
}

function FonctionOFF() {
    ws.send("OFF");
    console.log("OFF");
}