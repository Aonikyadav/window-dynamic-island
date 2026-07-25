import React from 'react';

export function MediaMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{
        display: 'grid',
        gridTemplateColumns: 'auto 1fr auto',
        gap: '1.2rem',
        padding: '1.2rem',
        width: '100%',
        height: '100%'
      }}>
        <div style={{
          width: '56px',
          height: '56px',
          borderRadius: '8px',
          background: 'linear-gradient(135deg, var(--accent), var(--violet-primary))',
          boxShadow: '0 4px 12px var(--accent-glow)',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          fontSize: '1.5rem'
        }}>
          🎵
        </div>
        <div style={{ display: 'flex', flexDirection: 'column', justifyContent: 'center' }}>
          <div style={{ fontWeight: 700, fontSize: '0.9rem', marginBottom: '0.2rem' }}>Desire (Rosé Theme)</div>
          <div style={{ fontSize: '0.75rem', color: 'var(--text-muted)', marginBottom: '0.5rem' }}>Goku Black</div>
          <div style={{ width: '100%', height: '3px', background: 'rgba(255, 255, 255, 0.15)', borderRadius: '99px' }}>
            <div style={{ width: '42%', height: '100%', background: 'var(--accent)', borderRadius: '99px' }} />
          </div>
        </div>
        <div style={{
          gridColumn: '1 / 4',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          gap: '1.5rem',
          borderTop: '1px solid rgba(255, 255, 255, 0.05)',
          paddingTop: '0.6rem'
        }}>
          <i class="fa-solid fa-backward-step" style={{ color: 'var(--text-muted)', cursor: 'pointer' }} />
          <i class="fa-solid fa-circle-pause" style={{ fontSize: '1.4rem', color: 'var(--text-bright)', cursor: 'pointer' }} />
          <i class="fa-solid fa-forward-step" style={{ color: 'var(--text-muted)', cursor: 'pointer' }} />
        </div>
      </div>
    );
  }

  return (
    <div style={{
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'space-between',
      width: '100%',
      height: '100%',
      padding: '0 0.8rem'
    }}>
      <div style={{
        width: '26px',
        height: '26px',
        borderRadius: '50%',
        background: 'linear-gradient(45deg, var(--accent), #000)',
        boxShadow: '0 0 8px var(--accent-glow)',
        animation: 'rotate-spinner 6s linear infinite'
      }} />
      <div style={{ width: 'calc(100% - 80px)', overflow: 'hidden', whiteSpace: 'nowrap' }}>
        <span style={{
          display: 'inline-block',
          animation: 'marquee 8s linear infinite',
          fontSize: '0.75rem',
          fontWeight: 600
        }}>
          Desire — Goku Black Rosé Theme Song
        </span>
      </div>
      <div style={{ display: 'flex', alignItems: 'center', gap: '2px' }}>
        <span style={{ width: '2px', height: '8px', background: 'var(--accent)', borderRadius: '1px', animation: 'wave-bar 1.2s infinite ease-in-out' }} />
        <span style={{ width: '2px', height: '14px', background: 'var(--accent)', borderRadius: '1px', animation: 'wave-bar 1.2s infinite ease-in-out 0.2s' }} />
        <span style={{ width: '2px', height: '18px', background: 'var(--accent)', borderRadius: '1px', animation: 'wave-bar 1.2s infinite ease-in-out 0.4s' }} />
      </div>
    </div>
  );
}
