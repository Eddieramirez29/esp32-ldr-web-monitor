#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your_SSID";
const char* password = "Your_password";

WebServer server(80);
const int ldrPin = 32;

// Insert HTML, CSS and JS
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Visualizador de Luz (LDR)</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap" rel="stylesheet">
<style>
:root {
    --sky-day: #87CEEB;
    --sun-color: #FFD700;
    --text-day: #2c3e50;
    --sky-night: #0c1445;
    --moon-color: #f4f4f4;
    --text-night: #f0f0f0;
}
body {
    font-family: 'Inter', sans-serif;
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    margin: 0;
    background-color: var(--sky-night);
    color: var(--text-night);
    transition: background-color 1.5s ease-in-out, color 1.5s ease-in-out;
    overflow: hidden;
    position: relative;
}
#stars-container {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    pointer-events: none;
    opacity: 1;
    transition: opacity 1.5s ease-in-out;
}
.star {
    position: absolute;
    background-color: white;
    border-radius: 50%;
    animation: twinkle 4s infinite ease-in-out;
}
@keyframes twinkle {
    0%, 100% { opacity: 0.5; transform: scale(0.8); }
    50% { opacity: 1; transform: scale(1); }
}
.scene-container {
    text-align: center;
}
#celestial-body {
    width: 250px;
    height: 250px;
    border-radius: 50%;
    background-color: var(--moon-color);
    display: flex;
    justify-content: center;
    align-items: center;
    position: relative;
    transition: background-color 1.5s ease-in-out, box-shadow 1.5s ease-in-out;
    box-shadow: 0 0 20px 5px rgba(244, 244, 244, 0.3);
}
#percentage-container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
}
#percentage {
    font-size: 4rem;
    font-weight: 700;
    margin: 0;
    text-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
}
#status-text {
    font-size: 1.2rem;
    font-weight: 400;
    margin-top: 5px;
    text-transform: uppercase;
    letter-spacing: 2px;
}
#simulation-notice {
    position: absolute;
    bottom: 20px;
    width: 100%;
    text-align: center;
    color: #fff;
    background-color: rgba(0,0,0,0.3);
    padding: 10px;
    display: none;
}
</style>
</head>
<body>
<div id="stars-container"></div>
<div class="scene-container">
    <div id="celestial-body">
        <div id="percentage-container">
            <span id="percentage">0%</span>
            <span id="status-text">Noche</span>
        </div>
    </div>
</div>
<div id="simulation-notice">
    Modo simulación: Mueve el mouse para cambiar la luz.
</div>
<script>
const body = document.body;
const celestialBody = document.getElementById("celestial-body");
const percentageText = document.getElementById("percentage");
const statusText = document.getElementById("status-text");
const starsContainer = document.getElementById("stars-container");
const simulationNotice = document.getElementById("simulation-notice");
let simulationMode = false;
function createStars() {
    const starCount = 100;
    for (let i = 0; i < starCount; i++) {
        const star = document.createElement("div");
        star.className = "star";
        const size = Math.random() * 2 + 1;
        star.style.width = `${size}px`;
        star.style.height = `${size}px`;
        star.style.top = `${Math.random() * 100}%`;
        star.style.left = `${Math.random() * 100}%`;
        star.style.animationDelay = `${Math.random() * 4}s`;
        starsContainer.appendChild(star);
    }
}
function updateUI(pct) {
    percentageText.innerText = pct + "%";
    const transitionFactor = pct / 100;
    const skyHue = 225 - 30 * transitionFactor;
    const skySaturation = 50 + 20 * transitionFactor;
    const skyLightness = 15 + 55 * transitionFactor;
    body.style.backgroundColor = `hsl(${skyHue}, ${skySaturation}%, ${skyLightness}%)`;
    if (pct >= 50) {
        const sunBrightness = 50 + (pct - 50) / 2;
        const glowSize = 10 + (pct - 50);
        celestialBody.style.backgroundColor = `hsl(50, 100%, ${sunBrightness}%)`;
        celestialBody.style.boxShadow = `0 0 ${glowSize}px ${glowSize / 3}px hsla(50, 100%, ${sunBrightness}%, 0.7)`;
        body.style.color = 'var(--text-day)';
        statusText.innerText = "Día";
        starsContainer.style.opacity = 1 - (pct - 50) / 50;
    } else {
        const moonBrightness = 90 + (pct / 5);
        const glowSize = 10 + pct / 2;
        celestialBody.style.backgroundColor = `hsl(220, 30%, ${moonBrightness}%)`;
        celestialBody.style.boxShadow = `0 0 ${glowSize}px ${glowSize / 5}px hsla(220, 30%, ${moonBrightness}%, 0.5)`;
        body.style.color = 'var(--text-night)';
        statusText.innerText = "Noche";
        starsContainer.style.opacity = 1;
    }
}
function updateLDR() {
    if (simulationMode) return;
    fetch("/ldr")
        .then(response => {
            if (!response.ok) throw new Error('Network error');
            return response.text();
        })
        .then(data => {
            const val = parseInt(data);
            const pct = Math.round((val / 4095) * 100);
            updateUI(pct);
        })
        .catch(() => {
            console.warn("No se pudo conectar con el dispositivo. Activando modo simulación.");
            activateSimulationMode();
        });
}
function activateSimulationMode() {
    simulationMode = true;
    simulationNotice.style.display = 'block';
    window.addEventListener('mousemove', (event) => {
        const pct = Math.round((event.clientX / window.innerWidth) * 100);
        updateUI(pct);
    });
}
createStars();
setInterval(updateLDR, 500);
updateLDR();
</script>
</body>
</html>
)rawliteral";

// Handlers
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleLDR() {
  int valor = analogRead(ldrPin);
  server.send(200, "text/plain", String(valor));
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/ldr", handleLDR);
  server.begin();
}

void loop() {
  server.handleClient();
}
