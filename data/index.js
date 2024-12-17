window.addEventListener('load', onLoad);

function onLoad(event) {
  initWebSocket();
}

function initWebSocket() {
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket = new WebSocket(gateway);

  websocket.onopen = function(event) {
    console.log('Connexion WebSocket ouverte');
  };

  websocket.onclose = function(event) {
    console.log('Connexion WebSocket fermée');
    setTimeout(initWebSocket, 2000);
  };

  websocket.onmessage = function(event) {
    var valeur = event.data;
    document.getElementById('compteur').innerHTML = compteur;
  };
}
