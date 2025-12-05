#include <WiFi.h>
#include <WebServer.h>
#include "esp_wifi.h"
#include <DNSServer.h>

WebServer server(80);
DNSServer dnsServer;

const char* AP_SSID = "Mm_wifi";
const char* AP_PASSWORD = "12345678";
const char* DOMINIO = "magic.mazo";
int numCartas = 1;

// Configuración de redes WiFi
bool transmitiendo = false;

// Beacon Packet buffer
uint8_t packet[128] = {
  0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04,
  0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x64, 0x00, 0x31, 0x04, 0x00
};

String cartasSeleccionadas[10];
char ssids[500][32];
int totalRedesActivas = 0;

// combinación de caracteres invisibles
const char* invisibles[] = {
  "\u200B", "\u2060", "\u200C", "\uFEFF", "\u200D",
  "\u200B\u2060", "\u200C\uFEFF", "\u200D\u200B", "\u2060\u200C", "\uFEFF\u200D",
  "\u200B\u200C", "\u200D\u2060", "\uFEFF\u200B", "\u200C\u200D", "\u2060\uFEFF",
  "\u200B\u200D", "\u200C\u2060", "\u200D\uFEFF", "\u2060\u200B", "\uFEFF\u200C",
  "\u200B\uFEFF", "\u200C\u200B", "\u200D\u200C", "\u2060\u200D", "\uFEFF\u2060",
  "\u200B\u200C\u200D", "\u200D\u2060\uFEFF", "\u2060\uFEFF\u200B", "\uFEFF\u200B\u200C", "\u200B\u200D\u2060"
};

// =================== HTML ===================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>MOE HACK</title>
<style>
  body {
    margin:0; padding:0; text-align:center; font-family:Arial, Helvetica, sans-serif; color:#222;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    min-height: 100vh;
  }
  .wrap {
    background: rgba(255,255,255,0.95);
    margin: 10px auto;
    padding: 12px;
    border-radius: 12px;
    max-width: 980px;
    box-shadow: 0 8px 32px rgba(0,0,0,0.1);
  }
  h1 {
    font-size: 6vw;
    margin: 8px 0 6px;
    letter-spacing: 2px;
    background: linear-gradient(45deg, #ff6b6b, #4ecdc4);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
  }

  .config-section {
    background: rgba(240,240,240,0.9);
    padding: 12px;
    border-radius: 10px;
    margin: 10px auto;
    max-width: 95%;
  }
  .config-row {
    display: flex;
    justify-content: center;
    align-items: center;
    gap: 10px;
    margin: 8px 0;
    flex-wrap: wrap;
  }
  select, .config-btn {
    padding: 8px 12px;
    border: 2px solid #444;
    border-radius: 8px;
    background: #fff;
    font-size: 3.5vw;
    cursor: pointer;
  }

  .suits {
    display:flex;
    justify-content:space-around;
    align-items:center;
    margin: 8px auto;
    max-width: 95%;
    gap:8px;
  }
  .suits button {
    flex:1;
    font-size:12vw;
    line-height:1;
    aspect-ratio:1/1;
    border:none;
    border-radius:12px;
    background:#fff;
    cursor:pointer;
    box-shadow:0 2px 8px rgba(0,0,0,.15);
    transition: transform 0.2s, box-shadow 0.2s;
  }
  .suits button:hover {
    transform: translateY(-2px);
    box-shadow:0 4px 12px rgba(0,0,0,.2);
  }
  .suits button:active { transform: scale(.97); }
  .spade   { color:black; border:3px solid black; }
  .heart   { color:red;   border:3px solid red; }
  .diamond { color:red;   border:3px solid red; }
  .club    { color:black; border:3px solid black; }

  .ranks {
    display:flex;
    flex-wrap:wrap;
    justify-content:center;
    gap:6px;
    max-width: 96%;
    margin: 10px auto;
  }
  .ranks button {
    width:15vw;
    max-width:70px;
    height:15vw;
    max-height:70px;
    font-size:5vw;
    border:2px solid #555;
    border-radius:8px;
    background:#eee;
    cursor:pointer;
    transition: background 0.2s;
  }
  .ranks button:active { background:#ddd; }
  #ranksSection { display:none; }

  .custom-box {
    margin: 12px auto 6px;
    max-width: 95%;
    display:flex;
    gap:6px;
    justify-content:center;
    align-items:center;
    flex-wrap:wrap;
  }
  .custom-box input {
    width: 60vw;
    max-width: 380px;
    font-size: 4vw;
    padding: 8px 10px;
    border:2px solid #444;
    border-radius: 8px;
    background: #fff;
  }
  .custom-box button {
    font-size: 4vw;
    padding: 8px 12px;
    border-radius: 8px;
    border: none;
    background: #111;
    color: #fff;
    cursor:pointer;
    box-shadow:0 2px 8px rgba(0,0,0,.15);
  }

  .selected-cards {
    background: rgba(255,255,240,0.9);
    padding: 12px;
    border-radius: 8px;
    margin: 12px auto;
    max-width: 95%;
    border: 2px solid #ddd;
    min-height: 50px;
  }
  .card-item {
    display: inline-block;
    background: white;
    padding: 6px 12px;
    margin: 4px;
    border-radius: 6px;
    border: 2px solid #444;
    font-weight: bold;
    font-size: 3.5vw;
  }
  .remove-card {
    background: #dc3545;
    color: white;
    border: none;
    border-radius: 50%;
    width: 18px;
    height: 18px;
    margin-left: 6px;
    cursor: pointer;
    font-size: 10px;
    line-height: 1;
  }

  .control-buttons {
    display: flex;
    justify-content: center;
    gap: 12px;
    margin: 12px 0;
  }
  .ctrl-btn {
    padding: 10px 20px;
    border: none;
    border-radius: 8px;
    color: white;
    font-size: 4vw;
    cursor: pointer;
    box-shadow: 0 2px 6px rgba(0,0,0,0.2);
    transition: transform 0.2s;
  }
  .ctrl-btn:active {
    transform: scale(0.95);
  }
  .start-btn { background: #28a745; }
  .stop-btn { background: #dc3545; }
  .clear-btn { background: #ffc107; color: black; }

  .status {
    padding: 8px;
    margin: 8px 0;
    border-radius: 6px;
    font-weight: bold;
    background: #f8f9fa;
    border: 2px solid #dee2e6;
    font-size: 3.5vw;
  }
  .status-active { background: #d4edda; color: #155724; border-color: #c3e6cb; }
  .status-inactive { background: #f8d7da; color: #721c24; border-color: #f5c6cb; }

  .developer {
    margin-top: 15px;
    padding: 8px;
    font-size: 12px;
    color: #666;
    border-top: 1px solid #ddd;
  }

  @media (min-width: 640px) {
    h1 { font-size: 38px; }
    .suits button { font-size: 80px; }
    .ranks button { font-size: 22px; width: 60px; height: 60px; }
    .custom-box input { font-size: 18px; }
    .custom-box button { font-size: 18px; }
    select, .config-btn { font-size: 16px; }
    .ctrl-btn { font-size: 18px; }
    .card-item { font-size: 16px; }
    .status { font-size: 16px; }
  }

  .hint { font-size: 12px; opacity:.85; margin-top: 6px; }
</style>
<script>
let currentSuit = '';
let currentCartas = 1;
let selectedCards = [];

// Redirigir automáticamente si estamos en la IP
if (window.location.hostname === '192.168.4.1') {
  window.location.href = 'http://magic.mazo';
}

function updateCartasCount() {
  currentCartas = parseInt(document.getElementById('cartasCount').value);
  selectedCards = [];
  updateUI();
}

function updateUI() {
  const cardsList = document.getElementById('selectedCardsList');
  const hintText = document.getElementById('hintText');
  const cardsInfo = document.getElementById('cardsInfo');

  if (currentCartas == 1) {
    hintText.innerHTML = "Selecciona 1 carta o escribe palabra (se verá 30 veces)";
  } else {
    hintText.innerHTML = "Selecciona " + currentCartas + " cartas diferentes (cada una se verá 1 vez)";
  }

  cardsList.innerHTML = '';
  if (selectedCards.length === 0) {
    cardsList.innerHTML = '<div style="color: #666; font-style: italic;">No hay cartas seleccionadas</div>';
  } else {
    selectedCards.forEach((card, index) => {
      const cardElement = document.createElement('div');
      cardElement.className = 'card-item';
      cardElement.innerHTML = card +
        '<button class="remove-card" onclick="removeCard(' + index + ')">×</button>';
      cardsList.appendChild(cardElement);
    });
  }

  cardsInfo.innerHTML = 'Seleccionadas: ' + selectedCards.length + ' / ' + currentCartas;

  if (selectedCards.length >= currentCartas) {
    document.getElementById('ranksSection').style.display = 'none';
  }
}

function chooseSuit(suit){
  if (selectedCards.length >= currentCartas) {
    alert('Ya tienes todas las cartas seleccionadas. Borra alguna si quieres cambiar.');
    return;
  }

  currentSuit = suit;
  document.getElementById('ranksSection').style.display = 'flex';
}

function chooseRank(rank){
  const card = rank + ' ' + currentSuit;

  if (selectedCards.includes(card)) {
    alert('Esta carta ya está seleccionada');
    return;
  }

  if (currentCartas == 1) {
    selectedCards = [card];
  } else {
    if (selectedCards.length < currentCartas) {
      selectedCards.push(card);
    } else {
      alert('Ya has seleccionado el máximo de ' + currentCartas + ' cartas');
      return;
    }
  }

  updateUI();
  document.getElementById('ranksSection').style.display = 'none';
}

function addCustomCard(){
  const text = document.getElementById('customText').value.trim();
  if(!text){
    alert("Escribe una palabra.");
    return;
  }

  if (selectedCards.includes(text)) {
    alert('Esta palabra ya está seleccionada');
    document.getElementById('customText').value = '';
    return;
  }

  if (currentCartas == 1) {
    selectedCards = [text];
  } else {
    if (selectedCards.length < currentCartas) {
      selectedCards.push(text);
    } else {
      alert('Ya has seleccionado el máximo de ' + currentCartas + ' cartas');
      return;
    }
  }

  document.getElementById('customText').value = '';
  updateUI();
}

function removeCard(index) {
  selectedCards.splice(index, 1);
  updateUI();
}

function clearAllCards() {
  selectedCards = [];
  updateUI();
}

function startTransmission(){
  if (selectedCards.length === 0) {
    alert('Selecciona al menos una carta o palabra');
    return;
  }

  if (selectedCards.length !== currentCartas) {
    alert('Debes seleccionar exactamente ' + currentCartas + ' cartas/palabras. Actualmente tienes ' + selectedCards.length);
    return;
  }

  const cardsParam = selectedCards.join('|');

  fetch('/startTransmission?cards=' + encodeURIComponent(cardsParam) + '&count=' + currentCartas)
    .then(response => {
      if (!response.ok) {
        throw new Error('Error en la respuesta del servidor');
      }
      return response.text();
    })
    .then(data => {
      let statusText = '🟢 TRANSMITIENDO: ';
      if (currentCartas == 1) {
        statusText += selectedCards[0] + ' (30 redes)';
      } else {
        statusText += selectedCards.length + ' redes diferentes';
      }
      document.getElementById('status').innerHTML = statusText;
      document.getElementById('status').className = 'status status-active';
    })
    .catch(error => {
      alert('Error al iniciar: ' + error.message);
    });
}

function stopTransmission(){
  fetch('/stopTransmission')
    .then(response => response.text())
    .then(data => {
      document.getElementById('status').innerHTML = '🔴 TRANSMISIÓN DETENIDA';
      document.getElementById('status').className = 'status status-inactive';
    });
}

document.addEventListener('DOMContentLoaded', function() {
  updateUI();
});
</script>
</head>
<body>
  <div class="wrap">
    <h1>MOE HACK</h1>

    <div class="status status-inactive" id="status">
      🔴 INACTIVO - Selecciona cartas y pulsa INICIAR
    </div>

    <div class="config-section">
      <h3>Número de Cartas/Palabras</h3>
      <div class="config-row">
        <select id="cartasCount" onchange="updateCartasCount()">
          <option value="1" selected>1 Carta</option>
          <option value="2">2 Cartas</option>
          <option value="3">3 Cartas</option>
          <option value="4">4 Cartas</option>
          <option value="5">5 Cartas</option>
          <option value="6">6 Cartas</option>
          <option value="7">7 Cartas</option>
          <option value="8">8 Cartas</option>
          <option value="9">9 Cartas</option>
          <option value="10">10 Cartas</option>
        </select>
      </div>
      <div class="hint" id="hintText">
        Selecciona 1 carta o escribe palabra (se verá 30 veces)
      </div>
    </div>

    <div class="selected-cards">
      <strong>Cartas Seleccionadas:</strong>
      <div id="cardsInfo" style="font-size: 11px; color: #666; margin-bottom: 6px;">Seleccionadas: 0 / 1</div>
      <div id="selectedCardsList">
        <div style="color: #666; font-style: italic;">No hay cartas seleccionadas</div>
      </div>
    </div>

    <div class="suits">
      <button class="spade"   onclick="chooseSuit('♠')">&spades;</button>
      <button class="heart"   onclick="chooseSuit('♥')">&hearts;</button>
      <button class="diamond" onclick="chooseSuit('♦')">&diams;</button>
      <button class="club"    onclick="chooseSuit('♣')">&clubs;</button>
    </div>

    <div id="ranksSection" class="ranks">
      <button onclick="chooseRank('A')">A</button><button onclick="chooseRank('2')">2</button>
      <button onclick="chooseRank('3')">3</button><button onclick="chooseRank('4')">4</button>
      <button onclick="chooseRank('5')">5</button><button onclick="chooseRank('6')">6</button>
      <button onclick="chooseRank('7')">7</button><button onclick="chooseRank('8')">8</button>
      <button onclick="chooseRank('9')">9</button><button onclick="chooseRank('10')">10</button>
      <button onclick="chooseRank('J')">J</button><button onclick="chooseRank('Q')">Q</button>
      <button onclick="chooseRank('K')">K</button>
    </div>

    <div class="custom-box">
      <input id="customText" type="text" maxlength="32" placeholder="Escribe palabra personalizada">
      <button onclick="addCustomCard()">Añadir Palabra</button>
    </div>

    <div class="control-buttons">
      <button class="ctrl-btn start-btn" onclick="startTransmission()">▶ INICIAR</button>
      <button class="ctrl-btn stop-btn" onclick="stopTransmission()">⏹ DETENER</button>
      <button class="ctrl-btn clear-btn" onclick="clearAllCards()">🗑 BORRAR</button>
    </div>

    <div class="hint">
      <strong>Instrucciones:</strong><br>
      1. Selecciona número de cartas (1-10)<br>
      2. Para 1 carta: se verá 30 veces<br>
      3. Para 2+ cartas: cada una se verá 1 vez<br>
      4. Pulsa INICIAR para comenzar
    </div>

    <div class="developer">
      Desarrollado por Elitemagic
    </div>
  </div>
</body>
</html>
)rawliteral";

// =================== HELPERS ===================
String clampSSID(const String& s) {
  return (s.length() <= 32) ? s : s.substring(0, 32);
}

void limpiarRedesAnteriores() {
  for(int i = 0; i < 500; i++) {
    ssids[i][0] = '\0';
  }
  totalRedesActivas = 0;
  Serial.println("Redes anteriores limpiadas");
}

void generarVariacionesCarta(const String& baseName) {
  limpiarRedesAnteriores();

  for(int i = 0; i < 30; i++) {
    String nombre = baseName;
    nombre += invisibles[i % 30];

    if (nombre.length() > 32) {
      nombre = nombre.substring(0, 32);
    }

    nombre.toCharArray(ssids[i], 32);
  }
  totalRedesActivas = 30;
  Serial.println("Generadas 30 redes VISUALMENTE IGUALES de: " + baseName);
}

void configurarMultiplesCartas(String cardsParam, int count) {
  limpiarRedesAnteriores();

  Serial.println("Procesando múltiples cartas: " + cardsParam);

  int startIndex = 0;
  int cardIndex = 0;

  for(int i = 0; i <= cardsParam.length() && cardIndex < count; i++) {
    if(i == cardsParam.length() || cardsParam.charAt(i) == '|') {
      String card = cardsParam.substring(startIndex, i);
      card.trim();

      if(card.length() > 0) {
        card.toCharArray(ssids[cardIndex], 32);
        cartasSeleccionadas[cardIndex] = card;
        cardIndex++;
      }
      startIndex = i + 1;
    }
  }
  totalRedesActivas = cardIndex;
  Serial.println("Total redes configuradas: " + String(cardIndex));
}

// =================== HTTP HANDLERS ===================
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleSetCartas() {
  if (server.hasArg("count")) {
    numCartas = server.arg("count").toInt();
    if (numCartas < 1) numCartas = 1;
    if (numCartas > 10) numCartas = 10;
    Serial.println("Número de cartas cambiado a: " + String(numCartas));
  }
  server.send(200, "text/plain", "OK");
}

void handleStartTransmission() {
  Serial.println("=== INICIANDO TRANSMISIÓN ===");

  if (!server.hasArg("cards")) {
    server.send(400, "text/plain", "No hay cartas seleccionadas");
    return;
  }

  String cardsParam = server.arg("cards");
  int receivedCount = server.arg("count").toInt();

  Serial.println("Cartas recibidas: " + cardsParam);
  Serial.println("Número de cartas: " + String(receivedCount));

  if (receivedCount == 1) {
    generarVariacionesCarta(cardsParam);
  } else {
    configurarMultiplesCartas(cardsParam, receivedCount);
  }

  transmitiendo = true;
  Serial.println("Transmisión iniciada con " + String(totalRedesActivas) + " redes WiFi");
  server.send(200, "text/plain", "Transmisión iniciada: " + String(totalRedesActivas) + " redes");
}

void handleStopTransmission() {
  transmitiendo = false;
  limpiarRedesAnteriores();
  Serial.println("Transmisión detenida y redes limpiadas");
  server.send(200, "text/plain", "Transmisión detenida");
}

// =================== SETUP & LOOP ===================
void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_AP);
  esp_wifi_start();

  Serial.println("AP iniciado: " + String(AP_SSID) + " Contraseña: " + String(AP_PASSWORD));
  Serial.print("IP Local: ");
  Serial.println(WiFi.softAPIP());

  // ===== INICIAR SERVIDOR DNS =====
  dnsServer.start(53, DOMINIO, WiFi.softAPIP()); // Solo responde a magic.mazo
  Serial.println("Servidor DNS iniciado en puerto 53");
  Serial.println("¡Ahora puedes usar: " + String(DOMINIO));

  server.on("/", handleRoot);
  server.on("/setCartas", handleSetCartas);
  server.on("/startTransmission", handleStartTransmission);
  server.on("/stopTransmission", handleStopTransmission);

  server.begin();

  Serial.println("==========================================");
  Serial.println("🎴 MOE HACK - SERVIDOR ACTIVO 🎴");
  Serial.println("==========================================");
  Serial.println("📱 Conéctate al WiFi: Mm_wifi");
  Serial.println("🔑 Contraseña: 12345678");
  Serial.println("🌐 Ahora puedes usar:");
  Serial.println("   → magic.mazo  (¡SOLO ESCRIBE ESTO!)");
  Serial.println("   → http://192.168.4.1");
  Serial.println("   → Calculadora WiFi App");
  Serial.println("==========================================");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  if (transmitiendo) {
    for(int i = 0; i < totalRedesActivas; i++) {
      packet[10] = packet[16] = random(256);
      packet[11] = packet[17] = random(256);
      packet[12] = packet[18] = random(256);
      packet[13] = packet[19] = random(256);
      packet[14] = packet[20] = random(256);
      packet[15] = packet[21] = random(256);

      int ssidLen = strlen(ssids[i]);
      packet[37] = ssidLen;

      for(int j = 0; j < ssidLen; j++) {
        packet[38 + j] = ssids[i][j];
      }

      esp_wifi_80211_tx(WIFI_IF_AP, packet, 38 + ssidLen, false);

      if (numCartas == 1) {
        delay(2);
      } else {
        delay(20);
      }
    }
    delay(numCartas == 1 ? 10 : 50);
  }
  delay(10);
}
