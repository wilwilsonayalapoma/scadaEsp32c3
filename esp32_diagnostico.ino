#include <WiFi.h>
#include <HTTPClient.h>

// 📡 Configuración WiFi
const char* ssid     = "chuty";      // Cambia por tu red WiFi
const char* password = "123456789+";  // Clave de tu red

// 🌐 Servidor SCADA - Múltiples opciones
String serverName = "http://10.203.0.47/scada/";
String serverIP = "10.203.0.47";
int serverPort = 80;

// ⚙️ Pines
#define SENSOR_PIN 4   // Entrada del sensor KF-06 (digital)
#define RELAY1     5   // Salida para controlar motor (relay canal 1)

// 🔄 Variables de control
bool modoAuto = false;
bool relayEstado = false;
String ultimoComando = "";
bool serverOnline = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("🚀 ESP32C3 SCADA - Versión Diagnóstico");
  Serial.println("=====================================");

  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH);

  // 🚀 Conexión WiFi
  conectarWiFi();
  
  // 🔍 Diagnóstico completo
  diagnosticoCompleto();
}

void conectarWiFi() {
  Serial.println("📡 Conectando a WiFi: " + String(ssid));
  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi conectado!");
    Serial.println("IP ESP32C3: " + WiFi.localIP().toString());
    Serial.println("Gateway: " + WiFi.gatewayIP().toString());
    Serial.println("Red: " + WiFi.SSID());
    Serial.println("Señal: " + String(WiFi.RSSI()) + " dBm");
  } else {
    Serial.println("\n❌ Error al conectar WiFi");
    while(true) delay(1000);
  }
}

void diagnosticoCompleto() {
  Serial.println("\n🔍 DIAGNÓSTICO DE CONECTIVIDAD");
  Serial.println("==============================");
  
  // Test 1: Ping al gateway
  Serial.println("Test 1: Ping al Gateway...");
  if (WiFi.ping(WiFi.gatewayIP())) {
    Serial.println("✅ Gateway alcanzable");
  } else {
    Serial.println("❌ Gateway no alcanzable");
  }
  
  // Test 2: Ping al servidor
  Serial.println("Test 2: Ping al Servidor...");
  IPAddress serverAddr;
  serverAddr.fromString(serverIP);
  if (WiFi.ping(serverAddr)) {
    Serial.println("✅ Servidor alcanzable por ping");
  } else {
    Serial.println("❌ Servidor no alcanzable por ping");
  }
  
  // Test 3: Conexión HTTP básica
  Serial.println("Test 3: Conexión HTTP...");
  probarHTTP();
  
  // Test 4: Conexión directa por socket
  Serial.println("Test 4: Conexión Socket...");
  probarSocket();
  
  Serial.println("==============================");
}

void probarHTTP() {
  HTTPClient http;
  
  // Configuración extendida
  http.setTimeout(15000);
  http.setConnectTimeout(10000);
  http.setUserAgent("ESP32C3-SCADA/1.0");
  
  String url = serverName + "test_conexion.php?test=ESP32C3&debug=1";
  Serial.println("URL: " + url);
  
  http.begin(url);
  
  int httpCode = http.GET();
  Serial.println("Código HTTP: " + String(httpCode));
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("✅ Respuesta HTTP OK:");
      Serial.println(payload);
      serverOnline = true;
    } else {
      Serial.println("⚠️ HTTP responde pero con código: " + String(httpCode));
    }
  } else {
    Serial.println("❌ Error HTTP: " + http.errorToString(httpCode));
    
    // Códigos comunes de error
    switch(httpCode) {
      case HTTPC_ERROR_CONNECTION_REFUSED:
        Serial.println("  -> Conexión rechazada (Puerto cerrado o firewall)");
        break;
      case HTTPC_ERROR_SEND_HEADER_FAILED:
        Serial.println("  -> Error enviando header HTTP");
        break;
      case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
        Serial.println("  -> Error enviando datos");
        break;
      case HTTPC_ERROR_NOT_CONNECTED:
        Serial.println("  -> No conectado");
        break;
      case HTTPC_ERROR_CONNECTION_LOST:
        Serial.println("  -> Conexión perdida");
        break;
      case HTTPC_ERROR_NO_STREAM:
        Serial.println("  -> Sin stream");
        break;
      case HTTPC_ERROR_NO_HTTP_SERVER:
        Serial.println("  -> No hay servidor HTTP");
        break;
      case HTTPC_ERROR_TOO_LESS_RAM:
        Serial.println("  -> Poca memoria RAM");
        break;
      case HTTPC_ERROR_ENCODING:
        Serial.println("  -> Error de codificación");
        break;
      case HTTPC_ERROR_STREAM_WRITE:
        Serial.println("  -> Error escribiendo stream");
        break;
      case HTTPC_ERROR_READ_TIMEOUT:
        Serial.println("  -> Timeout de lectura");
        break;
      default:
        Serial.println("  -> Error desconocido");
    }
  }
  
  http.end();
}

void probarSocket() {
  WiFiClient client;
  
  Serial.println("Conectando a " + serverIP + ":" + String(serverPort));
  
  if (client.connect(serverIP.c_str(), serverPort)) {
    Serial.println("✅ Conexión socket exitosa");
    
    // Enviar request HTTP manual
    client.print("GET /scada/test_conexion.php HTTP/1.1\r\n");
    client.print("Host: " + serverIP + "\r\n");
    client.print("Connection: close\r\n\r\n");
    
    // Leer respuesta
    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 5000) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
  } else {
    Serial.println("❌ Conexión socket falló");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && serverOnline) {
    // Loop normal solo si el servidor está online
    ejecutarSCADA();
  } else {
    // Re-diagnosticar cada 30 segundos
    static unsigned long ultimoDiagnostico = 0;
    if (millis() - ultimoDiagnostico > 30000) {
      Serial.println("\n🔄 Re-intentando conectividad...");
      diagnosticoCompleto();
      ultimoDiagnostico = millis();
    }
  }
  
  delay(2000);
}

void ejecutarSCADA() {
  HTTPClient http;
  
  // 1) 📤 Enviar datos del sensor
  int sensor = digitalRead(SENSOR_PIN);
  String estadoSensor = (sensor == LOW) ? "ON" : "OFF";
  
  String urlUpdate = serverName + "update.php?sensor=" + estadoSensor;
  http.begin(urlUpdate);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Sensor: " + estadoSensor + " -> " + payload);
  } else {
    Serial.println("Error enviando sensor: " + String(httpCode));
    serverOnline = false;
    return;
  }
  http.end();
  
  // 2) 📥 Recibir órdenes
  http.begin(serverName + "get_commands.php");
  httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    
    if (payload != ultimoComando) {
      Serial.println("Comando: " + payload);
      ultimoComando = payload;
      
      // Procesar comandos...
      if (payload.indexOf("MODO=AUTO") >= 0) {
        modoAuto = true;
        Serial.println("🤖 Modo AUTO");
      } else if (payload.indexOf("MODO=MANUAL") >= 0) {
        modoAuto = false;
        Serial.println("🎮 Modo MANUAL");
      }
      
      if (modoAuto) {
        activarRelay(estadoSensor == "ON");
      } else {
        if (payload.indexOf("RELAY1=ON") >= 0) {
          activarRelay(true);
        } else if (payload.indexOf("RELAY1=OFF") >= 0) {
          activarRelay(false);
        }
      }
    }
  } else {
    Serial.println("Error recibiendo comandos: " + String(httpCode));
    serverOnline = false;
  }
  http.end();
}

void activarRelay(bool estado) {
  if (relayEstado != estado) {
    relayEstado = estado;
    digitalWrite(RELAY1, estado ? LOW : HIGH);
    
    // Reportar al servidor
    HTTPClient http;
    String urlRelay = serverName + "update.php?relay1=" + (estado ? "ON" : "OFF");
    http.begin(urlRelay);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Relay " + (estado ? "ON" : "OFF") + " -> " + payload);
    }
    http.end();
  }
}
