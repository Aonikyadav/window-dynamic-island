// ==========================================================================
// GOKU BLACK ROSÉ EDITION - JAVASCRIPT APP
// ==========================================================================

document.addEventListener('DOMContentLoaded', () => {
    // ----------------------------------------------------
    // State Variables for Interactive Simulator
    // ----------------------------------------------------
    let currentMode = 'idle';
    let isHovered = false;
    let sizeScale = 1.0;
    let pillOpacity = 0.95;
    let activeThemeColor = '#ff2a85';

    const island = document.getElementById('live-island');
    const desktopScreen = document.querySelector('.desktop-screen');

    // ----------------------------------------------------
    // Mode Render Templates
    // ----------------------------------------------------
    const templates = {
        idle: {
            collapsed: `
                <div class="sim-mode-idle">
                    <span class="time-text">10:09 PM</span>
                    <span style="opacity: 0.2;">|</span>
                    <span class="active-win"><i class="fa-solid fa-desktop"></i> window-dynamic-island</span>
                </div>
            `,
            expanded: `
                <div class="sim-mode-weather-expanded">
                    <div class="weather-expanded-left">
                        <div class="weather-expanded-temp">24°C</div>
                        <div class="weather-expanded-city">Mathura, IN</div>
                    </div>
                    <div class="weather-expanded-right">
                        <div class="weather-grid-item"><span>Wind</span><span>E 14 km/h</span></div>
                        <div class="weather-grid-item"><span>Humidity</span><span>62%</span></div>
                        <div class="weather-grid-item"><span>Feels Like</span><span>26°C</span></div>
                        <div class="weather-grid-item"><span>UV Index</span><span>1 (Low)</span></div>
                    </div>
                </div>
            `,
            width: { collapsed: 175, expanded: 420 },
            height: { collapsed: 36, expanded: 220 }
        },
        media: {
            collapsed: `
                <div class="sim-mode-media">
                    <div class="media-art-spinner"></div>
                    <div class="media-marquee-container">
                        <span class="media-marquee-text">Desire — Goku Black Rosé Theme Song</span>
                    </div>
                    <div class="media-waveform">
                        <span></span><span></span><span></span><span></span><span></span>
                    </div>
                </div>
            `,
            expanded: `
                <div class="sim-mode-media-expanded">
                    <div class="art-expanded"></div>
                    <div class="media-expanded-info">
                        <div class="media-expanded-title">Desire (Rosé Theme)</div>
                        <div class="media-expanded-artist">Goku Black</div>
                        <div class="media-progress-bar">
                            <div class="media-progress-current"></div>
                        </div>
                    </div>
                    <div class="media-expanded-controls">
                        <span class="media-ctrl-btn"><i class="fa-solid fa-backward-step"></i></span>
                        <span class="media-ctrl-btn play"><i class="fa-solid fa-circle-pause"></i></span>
                        <span class="media-ctrl-btn"><i class="fa-solid fa-forward-step"></i></span>
                    </div>
                </div>
            `,
            width: { collapsed: 205, expanded: 380 },
            height: { collapsed: 36, expanded: 184 }
        },
        weather: {
            collapsed: `
                <div class="sim-mode-weather">
                    <span class="weather-info"><i class="fa-solid fa-cloud-sun"></i> 24°C</span>
                    <span class="weather-location" style="opacity: 0.7;">Mathura</span>
                </div>
            `,
            expanded: `
                <div class="sim-mode-weather-expanded">
                    <div class="weather-expanded-left">
                        <div class="weather-expanded-temp">24°C</div>
                        <div class="weather-expanded-city">Mathura, IN</div>
                    </div>
                    <div class="weather-expanded-right">
                        <div class="weather-grid-item"><span>Wind</span><span>E 14 km/h</span></div>
                        <div class="weather-grid-item"><span>Humidity</span><span>62%</span></div>
                        <div class="weather-grid-item"><span>Feels Like</span><span>26°C</span></div>
                        <div class="weather-grid-item"><span>UV Index</span><span>1 (Low)</span></div>
                    </div>
                </div>
            `,
            width: { collapsed: 205, expanded: 420 },
            height: { collapsed: 36, expanded: 220 }
        },
        system: {
            collapsed: `
                <div class="sim-mode-idle">
                    <span>⚙️ System</span>
                    <span style="opacity: 0.2;">|</span>
                    <span>CPU 38%</span>
                </div>
            `,
            expanded: `
                <div class="sim-mode-system-expanded">
                    <div class="sys-grid-cards">
                        <div class="sys-card">
                            <span class="sys-card-title">CPU</span>
                            <span class="sys-card-value">38%</span>
                            <div class="sys-card-bar"><div class="sys-card-fill" style="width: 38%;"></div></div>
                        </div>
                        <div class="sys-card">
                            <span class="sys-card-title">RAM</span>
                            <span class="sys-card-value">54%</span>
                            <div class="sys-card-bar"><div class="sys-card-fill" style="width: 54%;"></div></div>
                        </div>
                        <div class="sys-card">
                            <span class="sys-card-title">GPU</span>
                            <span class="sys-card-value">22%</span>
                            <div class="sys-card-bar"><div class="sys-card-fill" style="width: 22%;"></div></div>
                        </div>
                    </div>
                </div>
            `,
            width: { collapsed: 195, expanded: 420 },
            height: { collapsed: 36, expanded: 220 }
        },
        stt: {
            collapsed: `
                <div class="sim-mode-stt">
                    <div style="display:flex; align-items:center; gap:0.4rem;">
                        <i class="fa-solid fa-microphone"></i>
                        <span>STT</span>
                    </div>
                    <div class="stt-mic-indicator"></div>
                </div>
            `,
            expanded: `
                <div class="sim-mode-stt-expanded">
                    <div class="stt-header">
                        <span style="font-size:0.75rem; font-weight:700;">Speech-to-Text</span>
                        <span class="stt-badge-live">LIVE</span>
                    </div>
                    <div class="stt-text-output">
                        Watch me transcribe speech in real time...<span class="stt-cursor"></span>
                    </div>
                    <div class="stt-actions">
                        <button class="stt-btn primary-stt">📋 Copy</button>
                        <button class="stt-btn">Clear</button>
                        <button class="stt-btn">Notepad ↗</button>
                    </div>
                </div>
            `,
            width: { collapsed: 180, expanded: 360 },
            height: { collapsed: 36, expanded: 160 }
        },
        assistant: {
            collapsed: `
                <div class="sim-mode-stt">
                    <div style="display:flex; align-items:center; gap:0.4rem;">
                        <i class="fa-solid fa-robot" style="color:var(--accent);"></i>
                        <span>Jarvis</span>
                    </div>
                    <div class="assistant-pulsing-orb"></div>
                </div>
            `,
            expanded: `
                <div class="sim-mode-assistant-expanded">
                    <div class="assistant-big-orb"></div>
                    <div class="assistant-text">Listening... Speak naturally</div>
                    <div class="assistant-waveform-container">
                        <span></span><span></span><span></span><span></span><span></span>
                    </div>
                </div>
            `,
            width: { collapsed: 185, expanded: 300 },
            height: { collapsed: 36, expanded: 160 }
        },
        battery: {
            collapsed: `
                <div class="sim-mode-battery">
                    <i class="fa-solid fa-battery-quarter"></i>
                    <span>Battery Low (18%)</span>
                </div>
            `,
            expanded: `
                <div class="sim-mode-battery-expanded">
                    <div class="battery-visual-box">
                        <div class="battery-fill"></div>
                    </div>
                    <span class="battery-expanded-text">Plug in Charger (18% Remaining)</span>
                </div>
            `,
            width: { collapsed: 180, expanded: 290 },
            height: { collapsed: 36, expanded: 110 }
        },
        clipboard: {
            collapsed: `
                <div class="sim-mode-clipboard">
                    <i class="fa-solid fa-clipboard"></i>
                    <span>Copied!</span>
                </div>
            `,
            expanded: `
                <div class="sim-mode-clipboard-expanded">
                    <div class="clipboard-expanded-icon">
                        <i class="fa-solid fa-paste"></i>
                    </div>
                    <div class="clipboard-expanded-info">
                        <span class="clipboard-title">Clipboard Preview</span>
                        <span class="clipboard-preview">github.com/Aonikyadav/window...</span>
                    </div>
                </div>
            `,
            width: { collapsed: 175, expanded: 340 },
            height: { collapsed: 36, expanded: 64 }
        }
    };

    // ----------------------------------------------------
    // Update Simulator Presentation
    // ----------------------------------------------------
    function updateSimulator() {
        const modeData = templates[currentMode];
        const state = isHovered ? 'expanded' : 'collapsed';
        
        // Injected HTML template
        island.innerHTML = modeData[state];
        
        // Target dimensions scaled
        const targetW = modeData.width[state] * sizeScale;
        const targetH = modeData.height[state] * sizeScale;
        
        // Apply dimensions and styling parameters
        island.style.width = `${targetW}px`;
        island.style.height = `${targetH}px`;
        island.style.opacity = pillOpacity;

        // Custom styling: adjust border-radius when expanded
        if (state === 'expanded') {
            island.style.borderRadius = '24px';
        } else {
            island.style.borderRadius = '99px';
        }
    }

    // ----------------------------------------------------
    // Hook Event Listeners
    // ----------------------------------------------------
    
    // Hovering simulation triggers expansion
    desktopScreen.addEventListener('mouseenter', () => {
        isHovered = true;
        updateSimulator();
    });

    desktopScreen.addEventListener('mouseleave', () => {
        isHovered = false;
        updateSimulator();
    });

    // Control buttons for switching modes
    const controlButtons = document.querySelectorAll('.ctrl-btn');
    controlButtons.forEach(btn => {
        btn.addEventListener('click', () => {
            controlButtons.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            
            currentMode = btn.getAttribute('data-mode');
            updateSimulator();
        });
    });

    // Customizer: Size Scale
    const sizeSlider = document.getElementById('size-scale-slider');
    const scaleValText = document.getElementById('scale-value');
    sizeSlider.addEventListener('input', (e) => {
        sizeScale = parseFloat(e.target.value);
        scaleValText.innerText = `${sizeScale.toFixed(1)}x`;
        updateSimulator();
    });

    // Customizer: Opacity
    const opacitySlider = document.getElementById('opacity-slider');
    const opacityValText = document.getElementById('opacity-value');
    opacitySlider.addEventListener('input', (e) => {
        const percent = parseInt(e.target.value);
        pillOpacity = percent / 100;
        opacityValText.innerText = `${percent}%`;
        updateSimulator();
    });

    // Customizer: Theme Accents Selection
    const themeDots = document.querySelectorAll('.theme-dot');
    themeDots.forEach(dot => {
        dot.addEventListener('click', () => {
            themeDots.forEach(d => d.classList.remove('active'));
            dot.classList.add('active');
            
            activeThemeColor = dot.getAttribute('data-color');
            document.documentElement.style.setProperty('--accent', activeThemeColor);
            updateSimulator();
        });
    });

    // Initial simulator trigger
    updateSimulator();

    // ----------------------------------------------------
    // SSJ Rosé Background Particles & Sparkles
    // ----------------------------------------------------
    const particlesContainer = document.querySelector('.aura-particles');
    const sparklesContainer = document.querySelector('.sparkles-container');

    function createParticle(container, isSparkle = false) {
        const particle = document.createElement('div');
        particle.classList.add(isSparkle ? 'sparkle' : 'aura-particle');
        
        const size = Math.random() * (isSparkle ? 6 : 14) + 2;
        particle.style.width = `${size}px`;
        particle.style.height = `${size}px`;
        
        // Random starting position
        particle.style.left = `${Math.random() * 100}%`;
        particle.style.bottom = `-20px`;
        
        // Color variance matching Goku Black Rosé aura
        const colorVar = Math.random();
        if (colorVar < 0.4) {
            particle.style.backgroundColor = 'var(--rose-primary)';
            particle.style.boxShadow = `0 0 ${size * 2}px var(--rose-primary)`;
        } else if (colorVar < 0.8) {
            particle.style.backgroundColor = 'var(--purple-primary)';
            particle.style.boxShadow = `0 0 ${size * 2}px var(--purple-primary)`;
        } else {
            particle.style.backgroundColor = '#ffffff';
            particle.style.boxShadow = `0 0 ${size * 2}px #ffffff`;
        }

        // Animated float duration & delay
        const duration = Math.random() * 6 + 4;
        particle.style.animation = `floatUp ${duration}s linear forwards`;
        
        container.appendChild(particle);
        
        // Clean up
        setTimeout(() => {
            particle.remove();
        }, duration * 1000);
    }

    // Styles for floating particles
    const styleSheet = document.createElement("style");
    styleSheet.innerText = `
        .aura-particles {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            pointer-events: none;
            overflow: hidden;
        }
        .aura-particle {
            position: absolute;
            border-radius: 50%;
            opacity: 0.7;
        }
        @keyframes floatUp {
            0% {
                transform: translateY(0) scale(1) rotate(0deg);
                opacity: 0.7;
            }
            100% {
                transform: translateY(-110vh) scale(0.3) rotate(360deg);
                opacity: 0;
            }
        }
        .sparkles-container {
            position: absolute;
            width: 100%;
            height: 100%;
            top: 0;
            left: 0;
            pointer-events: none;
            z-index: 1;
        }
        .sparkle {
            position: absolute;
            border-radius: 50%;
            opacity: 0.9;
        }
    `;
    document.head.appendChild(styleSheet);

    // Spawning interval loops
    setInterval(() => createParticle(particlesContainer), 350);
    setInterval(() => createParticle(sparklesContainer, true), 800);

    // ----------------------------------------------------
    // Setup Instruction Tab Switching
    // ----------------------------------------------------
    const tabButtons = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');

    tabButtons.forEach(btn => {
        btn.addEventListener('click', () => {
            const target = btn.getAttribute('data-target');
            
            tabButtons.forEach(b => b.classList.remove('active'));
            tabContents.forEach(c => c.classList.remove('active'));
            
            btn.classList.add('active');
            document.getElementById(target).classList.add('active');
        });
    });

    // Populate windhawk code box preview
    const codeBox = document.getElementById('windhawk-install-code');
    if (codeBox) {
        codeBox.innerText = `// Win32 Standalone / Windhawk Injection Hook
#define WINDHAWK_META_NAME L"Window Dynamic Island"
#define WINDHAWK_META_VERSION L"1.4.1"
#define WINDHAWK_META_AUTHOR L"Aonik Yadav"

#include <windows.h>
#include <d2d1.h>
// Detailed standalone desktop code compiled natively...`;
    }
});

// Helper: Copy code block function
function copyCode() {
    const codeText = `// Win32 Standalone / Windhawk Injection Hook
#define WINDHAWK_META_NAME L"Window Dynamic Island"
#define WINDHAWK_META_VERSION L"1.4.1"
#define WINDHAWK_META_AUTHOR L"Aonik Yadav"

#include <windows.h>
#include <d2d1.h>
// Detailed standalone desktop code compiled natively...`;
    
    navigator.clipboard.writeText(codeText).then(() => {
        const btn = document.querySelector('.copy-code-btn');
        btn.innerHTML = `<i class="fa-solid fa-check"></i> Copied!`;
        setTimeout(() => {
            btn.innerHTML = `<i class="fa-regular fa-copy"></i> Copy`;
        }, 2000);
    }).catch(err => {
        console.error('Failed to copy text: ', err);
    });
}
