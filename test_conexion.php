<?php
// 🔍 Endpoint para verificar conectividad desde ESP32C3
header('Content-Type: text/plain');
header('Access-Control-Allow-Origin: *');

// Registrar la consulta
$ip_cliente = $_SERVER['REMOTE_ADDR'];
$timestamp = date('Y-m-d H:i:s');

echo "ESP32C3 Conectividad OK\n";
echo "IP Cliente: $ip_cliente\n";
echo "Timestamp: $timestamp\n";
echo "Servidor: " . $_SERVER['HTTP_HOST'] . "\n";

// Si se incluye un parámetro de prueba
if (isset($_GET['test'])) {
    echo "Parámetro test: " . $_GET['test'] . "\n";
}

// Verificar base de datos
include "db.php";
if ($conn->connect_error) {
    echo "DB Error: " . $conn->connect_error . "\n";
} else {
    echo "DB: Conectado OK\n";
}
$conn->close();
?>
