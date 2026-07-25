import React from 'react';

export function BatteryMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', padding: '1.2rem', width: '100%', height: '100%', gap: '0.8rem' }}>
        <div style={{ position: 'relative', width: '90px', height: '38px', border: '3px solid var(--text-bright)', borderRadius: '8px', padding: '3px', display: 'flex', alignItems: 'center' }}>
          <div style={{ width: '18%', height: '100%', background: 'var(--rose-primary)', borderRadius: '3px', animation: 'batt-glow 1s infinite alternate' }} />
        </div>
        <span style={{ fontSize: '0.8rem', fontWeight: 700 }}>Plug in Charger (18% Remaining)</span>
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'center', gap: '0.5rem', width: '100%', height: '100%', padding: '0 1rem', fontSize: '0.75rem', fontWeight: 600 }}>
      <i class="fa-solid fa-battery-quarter" style={{ color: 'var(--rose-primary)' }} />
      <span>Battery Low (18%)</span>
    </div>
  );
}
