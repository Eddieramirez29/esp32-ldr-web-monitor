const body = document.body;
const celestialBody = document.getElementById("celestial-body");
const percentageText = document.getElementById("percentage");
const statusText = document.getElementById("status-text");
const starsContainer = document.getElementById("stars-container");
const simulationNotice = document.getElementById("simulation-notice");
let simulationMode = false;

// Genera estrellas aleatoriamente
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

// Actualiza la UI según el valor del LDR
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

// Obtiene valor real del LDR
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

// Activa el modo simulación
function activateSimulationMode() {
    simulationMode = true;
    simulationNotice.style.display = 'block';
    window.addEventListener('mousemove', (event) => {
        const pct = Math.round((event.clientX / window.innerWidth) * 100);
        updateUI(pct);
    });
}

// Inicialización
createStars();
setInterval(updateLDR, 500);
updateLDR();
