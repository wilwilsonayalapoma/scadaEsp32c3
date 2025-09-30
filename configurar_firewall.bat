@echo off
echo =============================================
echo    CONFIGURADOR FIREWALL PARA ESP32C3 SCADA
echo =============================================
echo.
echo IMPORTANTE: Este script debe ejecutarse como ADMINISTRADOR
echo.

echo 1. Agregando regla de firewall para Apache HTTP...
netsh advfirewall firewall add rule name="Apache HTTP SCADA" dir=in action=allow protocol=TCP localport=80
echo.

echo 2. Agregando regla de firewall para Apache HTTPS...
netsh advfirewall firewall add rule name="Apache HTTPS SCADA" dir=in action=allow protocol=TCP localport=443
echo.

echo 3. Verificando reglas creadas...
netsh advfirewall firewall show rule name="Apache HTTP SCADA"
echo.

echo 4. Tu configuracion de red actual:
ipconfig | findstr "IPv4"
echo.

echo 5. Verificando que Apache este escuchando...
netstat -an | findstr :80
echo.

echo =============================================
echo CONFIGURACION COMPLETADA
echo =============================================
echo.
echo Ahora puedes:
echo 1. Reiniciar XAMPP (Apache)
echo 2. Cargar el codigo corregido al ESP32C3
echo 3. Verificar en Serial Monitor
echo.
echo URLs de prueba:
echo - http://localhost/scada/
echo - http://10.203.0.47/scada/
echo - http://localhost/scada/diagnostico.php
echo.

pause
