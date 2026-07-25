import React from 'react';

export function AssistantMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', padding: '1.2rem', width: '100%', height: '100%', gap: '0.8rem' }}>
        <div style={{
          width: '48px',
          height: '48px',
          borderRadius: '50%',
          background: 'radial-gradient(circle, var(--accent) 0%, var(--violet-primary) 100%)',
          boxShadow: '0 0 25px var(--accent)',
          animation: 'orb-pulse 2s infinite ease-in-out'
        }} />
        <div style={{ fontSize: '0.8rem', fontWeight: 500, textAlign: 'center' }}>Listening... Speak naturally</div>
        <div style={{ display: 'flex', alignItems: 'center', gap: '4px', height: '20px' }}>
          <span style={{ width: '3px', height: '4px', background: 'var(--accent)', borderRadius: '99px', animation: 'wave-bar 0.8s infinite ease-in-out' }} />
          <span style={{ width: '3px', height: '12px', background: 'var(--accent)', borderRadius: '99px', animation: 'wave-bar 0.8s infinite ease-in-out 0.1s' }} />
          <span style={{ width: '3px', height: '16px', background: 'var(--accent)', borderRadius: '99px', animation: 'wave-bar 0.8s infinite ease-in-out 0.3s' }} />
        </div>
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', height: '100%', padding: '0 1rem', fontSize: '0.75rem', fontWeight: 600 }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '0.4rem' }}>
        <i class="fa-solid fa-robot" style={{ color: 'var(--accent)' }} />
        <span>Jarvis</span>
      </div>
      <div style={{ width: '14px', height: '14px', borderRadius: '50%', background: 'radial-gradient(circle, var(--accent) 0%, var(--violet-primary) 100%)', boxShadow: '0 0 12px var(--accent)', animation: 'orb-pulse 1.2s infinite ease-in-out' }} />
    </div>
  );
}
