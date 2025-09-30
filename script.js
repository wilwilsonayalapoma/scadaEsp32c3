function actualizar() {
  // Obtener estado del relay con formato visual
  fetch("get_commands.php?web=1")
    .then((res) => res.text())
    .then((data) => {
      document.getElementById("estadoRelay").innerHTML = data;
    });

  // Obtener estado del sensor
  fetch("sensores_estado.php")
    .then((res) => res.text())
    .then((data) => {
      document.getElementById("sensor").innerHTML = data;
    });
}

function enviarOrden(nombre, estado) {
  fetch("ordenar.php?nombre=" + nombre + "&estado=" + estado)
    .then((res) => res.text())
    .then((data) => {
      alert("✅ " + data);
      actualizar(); // Actualizar inmediatamente después del comando
    });
}

function cambiarModo(modo) {
  fetch("ordenar.php?nombre=RELAY1&modo=" + modo)
    .then((res) => res.text())
    .then((data) => {
      alert("🔄 " + data);
      actualizar(); // Actualizar inmediatamente después del cambio
    });
}

// Actualizar cada 2 segundos
setInterval(actualizar, 2000);

// Actualizar inmediatamente al cargar la página
window.onload = function () {
  actualizar();
};
