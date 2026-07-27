import fs from 'fs';
import path from 'path';

// Let's create an SVG string for the Cyberpunk Rosé Dynamic Island icon
const svgContent = `<svg xmlns="http://www.w3.org/2000/svg" width="512" height="512" viewBox="0 0 512 512">
  <defs>
    <!-- Dark Background Gradient -->
    <linearGradient id="bgGrad" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" stop-color="#0a0a0f"/>
      <stop offset="50%" stop-color="#12111a"/>
      <stop offset="100%" stop-color="#050508"/>
    </linearGradient>

    <!-- Glowing Rose Pill Gradient -->
    <linearGradient id="roseGrad" x1="0%" y1="0%" x2="100%" y2="0%">
      <stop offset="0%" stop-color="#ff0055"/>
      <stop offset="50%" stop-color="#fb7185"/>
      <stop offset="100%" stop-color="#e11d48"/>
    </linearGradient>

    <!-- Cyan Camera Lens Gradient -->
    <linearGradient id="lensGrad" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" stop-color="#38bdf8"/>
      <stop offset="100%" stop-color="#0284c7"/>
    </linearGradient>

    <!-- Outer Neon Glow Filter -->
    <filter id="neonGlow" x="-50%" y="-50%" width="200%" height="200%">
      <feGaussianBlur stdDeviation="16" result="blur1"/>
      <feGaussianBlur stdDeviation="8" result="blur2"/>
      <feMerge>
        <feMergeNode in="blur1"/>
        <feMergeNode in="blur2"/>
        <feMergeNode in="SourceGraphic"/>
      </feMerge>
    </filter>

    <!-- Subtle Inner Shadow for Depth -->
    <filter id="glassReflect" x="0%" y="0%" width="100%" height="100%">
      <feLinearGradient id="reflectGrad" x1="0" y1="0" x2="0" y2="1">
        <stop offset="0%" stop-color="#ffffff" stop-opacity="0.3"/>
        <stop offset="100%" stop-color="#ffffff" stop-opacity="0.0"/>
      </feLinearGradient>
    </filter>
  </defs>

  <!-- Base Squircle Window Frame -->
  <rect x="24" y="24" width="464" height="464" rx="100" fill="url(#bgGrad)" stroke="#ff0055" stroke-width="4" stroke-opacity="0.4"/>
  <rect x="28" y="28" width="456" height="456" rx="96" fill="none" stroke="#ffffff" stroke-width="1.5" stroke-opacity="0.1"/>

  <!-- Background Aura Radial Glow -->
  <circle cx="256" cy="200" r="160" fill="#ff0055" opacity="0.18" filter="url(#neonGlow)"/>

  <!-- MAIN DYNAMIC ISLAND PILL CONTAINER -->
  <g filter="url(#neonGlow)">
    <!-- Pill Base Frame -->
    <rect x="86" y="156" width="340" height="120" rx="60" fill="#000000" stroke="url(#roseGrad)" stroke-width="3.5"/>
    
    <!-- Glossy Highlight Curve -->
    <path d="M 120 162 Q 256 175 392 162 A 54 54 0 0 1 420 196 Q 256 185 92 196 A 54 54 0 0 1 120 162 Z" fill="#ffffff" opacity="0.12"/>

    <!-- Left Hardware Camera Sensor Cutout -->
    <circle cx="146" cy="216" r="22" fill="#0c0a12" stroke="#ff0055" stroke-width="2"/>
    <circle cx="146" cy="216" r="12" fill="url(#lensGrad)"/>
    <circle cx="149" cy="213" r="4" fill="#ffffff" opacity="0.8"/>

    <!-- Center-Right Waveform Equalizer Bars -->
    <rect x="210" y="202" width="10" height="28" rx="5" fill="#ff0055"/>
    <rect x="230" y="186" width="10" height="60" rx="5" fill="#fb7185"/>
    <rect x="250" y="174" width="10" height="84" rx="5" fill="#ffffff"/>
    <rect x="270" y="190" width="10" height="52" rx="5" fill="#fb7185"/>
    <rect x="290" y="206" width="10" height="20" rx="5" fill="#ff0055"/>

    <!-- Right Privacy Alert Dot -->
    <circle cx="366" cy="216" r="12" fill="#22c55e" filter="url(#neonGlow)"/>
    <circle cx="366" cy="216" r="5" fill="#ffffff"/>
  </g>

  <!-- Bottom Accent Typography Indicator -->
  <text x="256" y="376" font-family="-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif" font-weight="800" font-size="34" letter-spacing="6" fill="#ffffff" text-anchor="middle" opacity="0.95">DYNAMIC ISLAND</text>
  <text x="256" y="412" font-family="-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif" font-weight="700" font-size="18" letter-spacing="4" fill="#ff0055" text-anchor="middle" opacity="0.9">FOR WINDOWS</text>
</svg>`;

// Save SVG to res/app_icon.svg and website/public/favicon.svg
const resDir = path.resolve('res');
if (!fs.existsSync(resDir)) fs.mkdirSync(resDir, { recursive: true });

fs.writeFileSync(path.join(resDir, 'app_icon.svg'), svgContent);

const websitePublic = path.resolve('website/public');
if (!fs.existsSync(websitePublic)) fs.mkdirSync(websitePublic, { recursive: true });

fs.writeFileSync(path.join(websitePublic, 'favicon.svg'), svgContent);

console.log('SVG icon generated successfully!');
