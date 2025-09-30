<?php
include "db.php";

if (isset($_GET['nombre'])) {
    // Validación y sanitización de datos
    $nombre = mysqli_real_escape_string($conn, $_GET['nombre']);
    $estado = isset($_GET['estado']) ? mysqli_real_escape_string($conn, $_GET['estado']) : 'OFF';
    $modo   = isset($_GET['modo']) ? mysqli_real_escape_string($conn, $_GET['modo']) : 'MANUAL';
    
    // Validar valores permitidos
    $estados_validos = ['ON', 'OFF'];
    $modos_validos = ['AUTO', 'MANUAL'];
    
    if (!in_array($estado, $estados_validos)) {
        $estado = 'OFF';
    }
    
    if (!in_array($modo, $modos_validos)) {
        $modo = 'MANUAL';
    }

    // Usar prepared statement para mayor seguridad
    $stmt = $conn->prepare("INSERT INTO ordenes (nombre, estado, modo) VALUES (?, ?, ?)");
    $stmt->bind_param("sss", $nombre, $estado, $modo);
    
    if ($stmt->execute()) {
        echo "Orden enviada: $nombre -> $estado ($modo)";
    } else {
        echo "Error al enviar orden";
    }
    
    $stmt->close();
}

$conn->close();
?>
