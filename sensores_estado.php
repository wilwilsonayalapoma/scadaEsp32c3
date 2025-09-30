<?php
include "db.php";

// Obtener el último valor del sensor
$stmt = $conn->prepare("SELECT nombre, valor, fecha FROM sensores WHERE nombre = ? ORDER BY id DESC LIMIT 1");
$sensor_name = 'SensorNivel';
$stmt->bind_param("s", $sensor_name);
$stmt->execute();
$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $icono = ($row['valor'] == 'ON') ? '🟢' : '🔴';
    $estado = ($row['valor'] == 'ON') ? 'DETECTADO' : 'SIN DETECCIÓN';
    
    echo "<div style='font-size: 18px;'>";
    echo "$icono <strong>Sensor KF-06</strong><br>";
    echo "Estado: <strong>$estado</strong><br>";
    echo "Última actualización: " . date('H:i:s', strtotime($row['fecha']));
    echo "</div>";
} else {
    echo "<div style='color: orange;'>⚠️ Sin datos del sensor</div>";
}

$stmt->close();
$conn->close();
?>
