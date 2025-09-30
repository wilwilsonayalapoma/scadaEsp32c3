# 🏭 Sistema SCADA ESP32C3

Sistema SCADA completo para monitoreo y control de procesos industriales usando ESP32C3, sensor KF-06 y interfaz web.

## 🚀 Características

- ✅ **Control Remoto**: Interfaz web para control de relé/motor
- ✅ **Monitoreo en Tiempo Real**: Sensor KF-06 de proximidad/nivel
- ✅ **Doble Modo**: Manual (botones web) y Automático (sensor)
- ✅ **Seguridad**: Prepared statements y validación de datos
- ✅ **Diagnóstico**: Herramientas de conectividad y troubleshooting
- ✅ **Responsive**: Interfaz adaptable a móviles y tablets

## 🛠️ Hardware Requerido

- **ESP32C3** - Microcontrolador con WiFi
- **Sensor KF-06** - Sensor de proximidad/nivel (Pin 4)
- **Relé** - Control de motor/bomba (Pin 5)
- **Fuente de alimentación** - Para ESP32C3 y relé

## 💻 Software Requerido

- **XAMPP** - Servidor Apache + MySQL + PHP
- **Arduino IDE** - Para programar ESP32C3
- **Navegador Web** - Para la interfaz SCADA

## 📦 Instalación

### 1. Configurar Servidor SCADA

```bash
# 1. Instalar XAMPP
# 2. Copiar archivos a: C:\xampp\htdocs\scada\
# 3. Importar base de datos:
mysql -u root < scada.sql
```

### 2. Configurar ESP32C3

```cpp
// Editar en esp32_mejorado.ino:
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";
String serverName = "http://TU_IP/scada/";
```

### 3. Configurar Firewall

```bash
# Ejecutar como Administrador:
configurar_firewall.bat
```

## 🌐 Uso del Sistema

### Acceso Web
- **Local**: http://localhost/scada/
- **Red**: http://TU_IP/scada/
- **Diagnóstico**: http://localhost/scada/diagnostico.php

### Modos de Operación

#### 🎮 Modo Manual
- Control via botones web
- ON/OFF inmediato
- Ideal para pruebas

#### 🤖 Modo Automático  
- Control por sensor KF-06
- Sin detección → Motor ON
- Con detección → Motor OFF
- Ideal para sistemas de nivel

## 👨‍💻 Autor

**Wilson Ayala Poma**
- GitHub: [@wilwilsonayalapoma](https://github.com/wilwilsonayalapoma)
- Email: wpoma248.doc@industrialmurillo.edu.bo

---

⭐ **¡Si te gusta este proyecto, dale una estrella!** ⭐
