<?php include "db.php"; ?>
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <title>SCADA - ESP32C3</title>
  <link rel="stylesheet" href="style.css">
  <script src="script.js"></script>
</head>
<body>
  <h1>SCADA Local - ESP32C3</h1>

  <div class="panel">
    <h2>Estado del Sensor</h2>
    <div id="sensor">Cargando...</div>
  </div>

  <div class="panel">
    <h2>Control del Motor</h2>
    <button onclick="enviarOrden('RELAY1','ON')">Encender</button>
    <button onclick="enviarOrden('RELAY1','OFF')">Apagar</button>
    <button onclick="cambiarModo('AUTO')">Automático</button>
    <button onclick="cambiarModo('MANUAL')">Manual</button>
    <div id="estadoRelay">Cargando...</div>
  </div>
</body>
</html>
