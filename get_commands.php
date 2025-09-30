<?php
include "db.php";

// Verificar si es una solicitud para mostrar en web
$isWebRequest = isset($_GET['web']) && $_GET['web'] == '1';

// Usar prepared statement para mayor seguridad
$stmt = $conn->prepare("SELECT nombre, estado, modo FROM ordenes WHERE nombre = ? ORDER BY id DESC LIMIT 1");
$relay_name = 'RELAY1';
$stmt->bind_param("s", $relay_name);
$stmt->execute();
$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    
    if ($isWebRequest) {
        // Respuesta visual para la interfaz web
        $iconoRelay = ($row['estado'] == 'ON') ? '🟢' : '🔴';
        $iconoModo = ($row['modo'] == 'AUTO') ? '🤖' : '🎮';
        $estadoTexto = ($row['estado'] == 'ON') ? 'ENCENDIDO' : 'APAGADO';
        
        echo "<div style='font-size: 18px;'>";
        echo "$iconoRelay <strong>Motor:</strong> $estadoTexto<br>";
        echo "$iconoModo <strong>Modo:</strong> " . $row['modo'];
        echo "</div>";
    } else {
        // Respuesta para ESP32C3
        echo "RELAY1=" . $row['estado'] . "&MODO=" . $row['modo'];
    }
} else {
    if ($isWebRequest) {
        echo "<div style='color: orange;'>⚠️ Sin datos del motor</div>";
    } else {
        echo "RELAY1=OFF&MODO=MANUAL";
    }
}

$stmt->close();
$conn->close();
?>
