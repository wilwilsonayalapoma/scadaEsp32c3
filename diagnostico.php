<?php
// 🔍 Diagnóstico de Conectividad SCADA
header('Content-Type: text/html; charset=UTF-8');
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Diagnóstico SCADA</title>
    <style>
        body { font-family: Arial; background: #f5f5f5; padding: 20px; }
        .panel { background: white; padding: 20px; margin: 10px 0; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }
        .success { color: #4CAF50; font-weight: bold; }
        .error { color: #f44336; font-weight: bold; }
        .info { color: #2196F3; font-weight: bold; }
    </style>
</head>
<body>
    <h1>🔍 Diagnóstico del Sistema SCADA</h1>
    
    <div class="panel">
        <h3>📡 Información del Servidor</h3>
        <p><span class="info">IP del Servidor:</span> <?php echo $_SERVER['SERVER_ADDR'] ?? 'No disponible'; ?></p>
        <p><span class="info">Puerto:</span> <?php echo $_SERVER['SERVER_PORT'] ?? '80'; ?></p>
        <p><span class="info">Host:</span> <?php echo $_SERVER['HTTP_HOST'] ?? 'localhost'; ?></p>
        <p><span class="info">Protocolo:</span> <?php echo isset($_SERVER['HTTPS']) ? 'HTTPS' : 'HTTP'; ?></p>
    </div>

    <div class="panel">
        <h3>🗄️ Estado de la Base de Datos</h3>
        <?php
        include "db.php";
        
        if ($conn->connect_error) {
            echo '<p class="error">❌ Error de conexión: ' . $conn->connect_error . '</p>';
        } else {
            echo '<p class="success">✅ Conexión a MySQL exitosa</p>';
            
            // Verificar tablas
            $tables = ['sensores', 'reles', 'ordenes'];
            foreach ($tables as $table) {
                $result = $conn->query("SHOW TABLES LIKE '$table'");
                if ($result->num_rows > 0) {
                    $count = $conn->query("SELECT COUNT(*) as total FROM $table")->fetch_assoc()['total'];
                    echo "<p class=\"success\">✅ Tabla '$table': $count registros</p>";
                } else {
                    echo "<p class=\"error\">❌ Tabla '$table' no encontrada</p>";
                }
            }
        }
        $conn->close();
        ?>
    </div>

    <div class="panel">
        <h3>🌐 URLs para ESP32C3</h3>
        <?php
        $server_ip = $_SERVER['SERVER_ADDR'] ?? 'localhost';
        $base_url = "http://" . $_SERVER['HTTP_HOST'];
        ?>
        <p><span class="info">URL Base:</span> <code><?php echo $base_url; ?>/scada/</code></p>
        <p><span class="info">Actualizar Sensor:</span> <code><?php echo $base_url; ?>/scada/update.php?sensor=ON</code></p>
        <p><span class="info">Obtener Comandos:</span> <code><?php echo $base_url; ?>/scada/get_commands.php</code></p>
        <p><span class="info">Actualizar Relé:</span> <code><?php echo $base_url; ?>/scada/update.php?relay1=ON</code></p>
    </div>

    <div class="panel">
        <h3>🔧 Configuración Recomendada ESP32C3</h3>
        <pre style="background: #f0f0f0; padding: 15px; border-radius: 5px;">
String serverName = "<?php echo $base_url; ?>/scada/";
        </pre>
    </div>

    <div class="panel">
        <h3>🔄 Prueba de Conectividad</h3>
        <button onclick="probarConexion()">Probar Conexión</button>
        <div id="resultado"></div>
    </div>

    <script>
        function probarConexion() {
            document.getElementById('resultado').innerHTML = '<p>⏳ Probando...</p>';
            
            fetch('get_commands.php')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('resultado').innerHTML = 
                        '<p class="success">✅ Conexión exitosa: ' + data + '</p>';
                })
                .catch(error => {
                    document.getElementById('resultado').innerHTML = 
                        '<p class="error">❌ Error de conexión: ' + error + '</p>';
                });
        }
    </script>
</body>
</html>
