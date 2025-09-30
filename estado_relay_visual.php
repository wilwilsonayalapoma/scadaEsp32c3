<?php
include "db.php";

// Obtener el último comando
$stmt = $conn->prepare("SELECT nombre, estado, modo FROM ordenes WHERE nombre = ? ORDER BY id DESC LIMIT 1");
$relay_name = 'RELAY1';
$stmt->bind_param("s", $relay_name);
$stmt->execute();
$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    
    // Para ESP32C3
    echo "RELAY1=" . $row['estado'] . "&MODO=" . $row['modo'];
    
} else {
    echo "RELAY1=OFF&MODO=MANUAL";
}

$stmt->close();
$conn->close();
?>
