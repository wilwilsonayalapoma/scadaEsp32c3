<?php
include "db.php";

if (isset($_GET['sensor'])) {
    $valor = mysqli_real_escape_string($conn, $_GET['sensor']);
    
    // Validar valores del sensor KF-06 (ON/OFF)
    $valores_validos = ['ON', 'OFF'];
    if (in_array($valor, $valores_validos)) {
        $stmt = $conn->prepare("INSERT INTO sensores (nombre, valor) VALUES (?, ?)");
        $nombre_sensor = 'SensorNivel';
        $stmt->bind_param("ss", $nombre_sensor, $valor);
        
        if ($stmt->execute()) {
            echo "Sensor actualizado: $valor";
        } else {
            echo "Error al actualizar sensor";
        }
        $stmt->close();
    } else {
        echo "Valor de sensor inválido";
    }
}

if (isset($_GET['relay1'])) {
    $estado = mysqli_real_escape_string($conn, $_GET['relay1']);
    
    // Validar estados permitidos
    $estados_validos = ['ON', 'OFF'];
    if (in_array($estado, $estados_validos)) {
        $stmt = $conn->prepare("INSERT INTO reles (nombre, estado) VALUES (?, ?)");
        $nombre_rele = 'Relay1';
        $stmt->bind_param("ss", $nombre_rele, $estado);
        
        if ($stmt->execute()) {
            echo "Relay1 actualizado: $estado";
        } else {
            echo "Error al actualizar relay";
        }
        $stmt->close();
    } else {
        echo "Estado de relay inválido";
    }
}

$conn->close();
?>
