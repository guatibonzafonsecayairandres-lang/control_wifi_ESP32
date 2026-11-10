wifi-two-led-lamp/
├── README.md
└── WifiLamp/
    └── WifiLamp.ino

/*
  Lámpara Wi-Fi de Dos Luces LED
  Control de 2 LEDs mediante Servidor Web local.
  Compatible con ESP8266 y ESP32.
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  WebServer server(80);
#endif

// --- CONFIGURACIÓN RED WI-FI ---
const char* ssid     = "TU_NOMBRE_DE_RED_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";

// --- CONFIGURACIÓN DE PINES ---
// Cambia los números de pin según tu placa y conexión
const int LED1_PIN = 4; // GPIO 4 (D2 en NodeMCU)
const int LED2_PIN = 5; // GPIO 5 (D1 en NodeMCU)

// Estado de los LEDs
bool led1State = false;
bool led2State = false;

// --- CÓDIGO HTML DE LA INTERFAZ WEB ---
String getHTMLPage() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>Lámpara Wi-Fi</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; margin-top: 50px; }";
  html += "h1 { color: #333; }";
  html += ".btn { display: inline-block; padding: 15px 30px; font-size: 18px; color: white; border: none; border-radius: 8px; text-decoration: none; margin: 10px; cursor: pointer; }";
  html += ".on { background-color: #28a745; }";
  html += ".off { background-color: #dc3545; }";
  html += ".card { background: white; padding: 20px; border-radius: 10px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); margin: 15px; }";
  html += "</style></head><body>";
  html += "<h1>Control de Lámpara Wi-Fi</h1>";
  
  // Tarjeta LED 1
  html += "<div class=\"card\"><h2>Luz 1 (Ambiente)</h2>";
  if (led1State) {
    html += "<p>Estado: <strong>ENCENDIDO</strong></p>";
    html += "<a href=\"/led1/off\"><button class=\"btn off\">Apagar</button></a>";
  } else {
    html += "<p>Estado: <strong>APAGADO</strong></p>";
    html += "<a href=\"/led1/on\"><button class=\"btn on\">Encender</button></a>";
  }
  html += "</div>";

  // Tarjeta LED 2
  html += "<div class=\"card\"><h2>Luz 2 (Lectura)</h2>";
  if (led2State) {
    html += "<p>Estado: <strong>ENCENDIDO</strong></p>";
    html += "<a href=\"/led2/off\"><button class=\"btn off\">Apagar</button></a>";
  } else {
    html += "<p>Estado: <strong>APAGADO</strong></p>";
    html += "<a href=\"/led2/on\"><button class=\"btn on\">Encender</button></a>";
  }
  html += "</div></body></html>";
  
  return html;
}

// --- MANEJADORES DE RUTAS WEB ---
void handleRoot() {
  server.send(200, "text/html", getHTMLPage());
}

void handleLed1On() {
  led1State = true;
  digitalWrite(LED1_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed1Off() {
  led1State = false;
  digitalWrite(LED1_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed2On() {
  led2State = true;
  digitalWrite(LED2_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed2Off() {
  led2State = false;
  digitalWrite(LED2_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Conexión a la red Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Conectado!");
  Serial.print("Dirección IP local: ");
  Serial.println(WiFi.localIP());

  // Definir rutas del servidor web
  server.on("/", handleRoot);
  server.on("/led1/on", handleLed1On);
  server.on("/led1/off", handleLed1Off);
  server.on("/led2/on", handleLed2On);
  server.on("/led2/off", handleLed2Off);

  server.begin();
  Serial.println("Servidor Web iniciado.");
}

void loop() {
  server.handleClient();
}
