#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your_SSID";
const char* password = "Your_password";

WebServer server(80);

const int ldrPin = 32;
const int lm35Pin = 36;

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Luz y Temperatura</title>
<style>
body
{
    font-family: Arial;
    display:flex;
    flex-direction:column;
    justify-content:center;
    align-items:center;
    height:100vh;
    margin:0;
    background:#10172a;
    color:#fff;
}
#container
{
    display:flex;
    gap:60px;
}
#celestial-body
{
    width:150px;
    height:150px;
    border-radius:50%;
    background:#ccc;
}
#percentage { font-size:2rem; font-weight:bold; }
#status-text { font-size:1rem; }
#thermo
{
    width:40px;
    height:200px;
    border:2px solid #fff;
    position:relative;
    border-radius:10px;
    overflow:hidden;
}
#liquid
{
    width:100%;
    height:0;
    background:red;
    position:absolute;
    bottom:0;
    transition:0.3s;
}
#temperature_text
{
    margin-top:10px;
    font-size:1.2rem;
}
</style>
</head>
<body>

<div id="container">
    <div>
        <div id="celestial-body"></div>
        <div id="percentage">0%</div>
        <div id="status-text">Noche</div>
    </div>

    <div>
        <div id="thermo">
            <div id="liquid"></div>
        </div>
        <div id="temperature_text">0°C</div>
    </div>
</div>

<script>
const body = document.body;
const celestialBody = document.getElementById("celestial-body");
const percentageText = document.getElementById("percentage");
const statusText = document.getElementById("status-text");
const liquid = document.getElementById("liquid");
const temperature_text = document.getElementById("temperature_text");

function updateUI(pct)
{
    percentageText.innerText = pct + "%";
    const t = pct / 100;
    const l = 15 + 55 * t;
    body.style.backgroundColor = `hsl(225,50%,${l}%)`;

    if (pct >= 50)
    {
        celestialBody.style.background = "#FFD700";
        statusText.innerText = "Día";
    }
    else
    {
        celestialBody.style.background = "#ccc";
        statusText.innerText = "Noche";
    }
}

function updateUITermometer(temp)
{
    temperature_text.innerText = temp + "°C";
    liquid.style.height = (temp) + "px";
}

function updateLDR()
{
    fetch("/ldr")
    .then(r => r.text())
    .then(data =>
    {
        const val = parseInt(data);
        const pct = (val / 4095) * 100;
        updateUI(pct.toFixed(2));
    });
}

function updateLM35()
{
    fetch("/lm35")
    .then(r => r.text())
    .then(data =>
    {
        const val = parseInt(data);
        const temp = (val * 3.3 * 100.0) / 4095;
        updateUITermometer(temp.toFixed(2));
    });
}

setInterval(() =>
{
    updateLDR();
    updateLM35();
}, 500);

updateLDR();
updateLM35();
</script>
</body>
</html>
)rawliteral";

void handleRoot()
{
    server.send(200, "text/html", htmlPage);
}

void handleLDR()
{
    int valor = analogRead(ldrPin);
    server.send(200, "text/plain", String(valor));
}

void handleLM35()
{
    int valor = analogRead(lm35Pin);
    server.send(200, "text/plain", String(valor));
}

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    server.on("/", handleRoot);
    server.on("/ldr", handleLDR);
    server.on("/lm35", handleLM35);
    server.begin();
}

void loop()
{
    server.handleClient();
}
