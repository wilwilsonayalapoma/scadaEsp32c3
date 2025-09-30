#include <WiFi.h>
#include <HTTPClient.h>

// 📡 Configuración WiFi
const char* ssid     = "chuty";      // Cambia por tu red WiFi
const char* password = "123456789+";  // Clave de tu red

// 🌐 Servidor SCADA (PC con XAMPP)
String serverName = "http://10.203.0.47/scada/";  // IP de tu laptop con XAMPP

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

  pinMode(SENSOR_PIN, INPUT_PULLUP); // KF-06 entrega contacto abierto/cerrado
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH); // Relé apagado (activo en LOW normalmente)

  // 🚀 Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
  
  // 🔍 Prueba de conectividad inicial
  probarConectividad();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Verificar conectividad cada 30 segundos si no está online
    static unsigned long ultimaVerificacion = 0;
    if (!serverOnline && millis() - ultimaVerificacion > 30000) {
      probarConectividad();
      ultimaVerificacion = millis();
    }
    
    // Solo ejecutar SCADA si el servidor está online
    if (serverOnline) {
      ejecutarSCADA();
    } else {
      Serial.println("⏳ Esperando servidor...");
    }
  } else {
    Serial.println("❌ WiFi desconectado");
  }

  delay(2000); // cada 2 segundos
}

void ejecutarSCADA() {
  HTTPClient http;

  // 1) 📤 Enviar datos del sensor
  int sensor = digitalRead(SENSOR_PIN); // 0 = activado, 1 = desactivado
  String estadoSensor = (sensor == LOW) ? "ON" : "OFF";

  String urlUpdate = serverName + "update.php?sensor=" + estadoSensor;
  http.begin(urlUpdate);
  http.setTimeout(5000); // 5 segundos timeout
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Sensor enviado: " + estadoSensor + " - Respuesta: " + payload);
  } else {
    Serial.println("❌ Error enviando sensor: " + String(httpCode));
    serverOnline = false;
    http.end();
    return;
  }
  http.end();

  // 2) 📥 Recibir órdenes desde SCADA
  http.begin(serverName + "get_commands.php");
  http.setTimeout(5000);
  
  httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    
    // Solo procesar si el comando cambió
    if (payload != ultimoComando) {
      Serial.println("Nuevo comando: " + payload);
      ultimoComando = payload;
      
      // Detectar modo de operación
      if (payload.indexOf("MODO=AUTO") >= 0) {
        modoAuto = true;
        Serial.println("🤖 Modo AUTOMÁTICO activado");
      } else if (payload.indexOf("MODO=MANUAL") >= 0) {
        modoAuto = false;
        Serial.println("🎮 Modo MANUAL activado");
      }

      // Procesar comandos según el modo
      if (modoAuto) {
        // En modo automático: relé se activa/desactiva según sensor
        if (estadoSensor == "ON") {
          activarRelay(true);
          Serial.println("🤖 AUTO: Sensor detectado → Motor ENCENDIDO");
        } else {
          activarRelay(false);
          Serial.println("🤖 AUTO: Sin detección → Motor APAGADO");
        }
      } else {
        // En modo manual: obedece comandos web
        if (payload.indexOf("RELAY1=ON") >= 0) {
          activarRelay(true);
          Serial.println("🎮 MANUAL: Motor ENCENDIDO por comando");
        } else if (payload.indexOf("RELAY1=OFF") >= 0) {
          activarRelay(false);
          Serial.println("🎮 MANUAL: Motor APAGADO por comando");
        }
      }
    }
  } else {
    Serial.println("❌ Error recibiendo comandos: " + String(httpCode));
    serverOnline = false;
  }
  http.end();
}

// 🔧 Función unificada para controlar relé
void activarRelay(bool estado) {
  if (relayEstado != estado) { // Solo cambiar si es diferente
    relayEstado = estado;
    digitalWrite(RELAY1, estado ? LOW : HIGH); // LOW=ON, HIGH=OFF
    
    // Reportar cambio al servidor
    enviarEstadoRelay(estado ? "ON" : "OFF");
  }
}

// 📤 Función para enviar estado del relé
void enviarEstadoRelay(String estado) {
  HTTPClient http;
  String urlRelay = serverName + "update.php?relay1=" + estado;
  http.begin(urlRelay);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Estado relay confirmado: " + estado + " - " + payload);
  } else {
    Serial.println("❌ Error confirmando relay: " + String(httpCode));
    serverOnline = false;
  }
  http.end();
}

// 🔍 Función para probar conectividad inicial
void probarConectividad() {
  Serial.println("🔍 Probando conectividad con servidor SCADA...");
  Serial.println("Red WiFi: " + WiFi.SSID());
  Serial.println("IP ESP32C3: " + WiFi.localIP().toString());
  Serial.println("Gateway: " + WiFi.gatewayIP().toString());
  Serial.println("DNS: " + WiFi.dnsIP().toString());
  
  // Test 1: Conexión TCP básica
  Serial.println("\n🔍 Test 1: Conexión TCP básica...");
  WiFiClient client;
  if (client.connect("10.203.0.47", 80)) {
    Serial.println("✅ Conexión TCP exitosa - Puerto 80 abierto");
    client.stop();
    
    // Test 2: HTTP Request
    Serial.println("🔍 Test 2: Request HTTP...");
    HTTPClient http;
    String urlTest = serverName + "test_conexion.php?test=ESP32C3";
    Serial.println("URL de prueba: " + urlTest);
    
    http.begin(urlTest);
    http.setTimeout(10000); // 10 segundos timeout
    
    int httpCode = http.GET();
    Serial.println("Código HTTP: " + String(httpCode));
    
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("✅ Respuesta del servidor:");
      Serial.println(payload);
      serverOnline = true;
      Serial.println("🎉 SERVIDOR ONLINE - Sistema SCADA listo!");
    } else {
      Serial.println("❌ Error HTTP: " + String(httpCode));
      serverOnline = false;
      mostrarErroresHTTP(httpCode);
    }
    http.end();
    
  } else {
    Serial.println("❌ Conexión TCP falló - Verificar:");
    Serial.println("1. Firewall de Windows");
    Serial.println("2. Estado de XAMPP/Apache");
    Serial.println("3. IP del servidor: 10.203.0.47");
    serverOnline = false;
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

// 🔍 Función para mostrar errores HTTP detallados
void mostrarErroresHTTP(int codigo) {
  Serial.println("📋 Análisis del error:");
  switch(codigo) {
    case -1:
      Serial.println("  -> CONNECTION_REFUSED: Firewall o Apache no responde");
      break;
    case -2:
      Serial.println("  -> SEND_HEADER_FAILED: Error enviando HTTP header");
      break;
    case -3:
      Serial.println("  -> SEND_PAYLOAD_FAILED: Error enviando datos");
      break;
    case -4:
      Serial.println("  -> NOT_CONNECTED: Sin conexión TCP");
      break;
    case -5:
      Serial.println("  -> CONNECTION_LOST: Conexión perdida");
      break;
    case -6:
      Serial.println("  -> NO_STREAM: Sin stream de datos");
      break;
    case -7:
      Serial.println("  -> NO_HTTP_SERVER: No hay servidor HTTP");
      break;
    case -8:
      Serial.println("  -> TOO_LESS_RAM: Memoria insuficiente");
      break;
    case -9:
      Serial.println("  -> ENCODING: Error de codificación");
      break;
    case -10:
      Serial.println("  -> STREAM_WRITE: Error escribiendo");
      break;
    case -11:
      Serial.println("  -> READ_TIMEOUT: Timeout de lectura");
      break;
    default:
      Serial.println("  -> Error desconocido: " + String(codigo));
  }
}
