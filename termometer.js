const liquid = document.getElementById("liquid");
const temperature_text = document.getElementById("temperature_text");



function updateUITermometer(pct)
{
    temperature_text.innerText = pct + "°C";
    liquid.style.height = pct + "px";
}


// Obtiene valor real del LDR
function updateLM35()
{
    fetch("/lm35")
        .then(response =>
        {
            if (!response.ok)
            {
                throw new Error("Network error");
            }
            return response.text();
        })
        .then(data =>
        {
            const val = parseInt(data);
            const pct = (val*3.3*100.0)/4095;
            const temperature_level = pct.toFixed(2);
            updateUITermometer(temperature_level);
        })
        .catch(() =>
        {
            console.warn("No se pudo conectar con el dispositivo LM35.");
        });
}

setInterval(updateLM35, 500);
updateLM35();