import React from 'react';

export function SystemMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{ padding: '1.2rem', width: '100%', height: '100%', display: 'flex', flexDirection: 'column', gap: '0.8rem' }}>
        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: '0.6rem' }}>
          <div style={{ background: 'rgba(255, 255, 255, 0.03)', border: '1px solid rgba(255, 255, 255, 0.05)', borderRadius: '8px', padding: '0.6rem', display: 'flex', flexDirection: 'column', gap: '0.2rem' }}>
            <span style={{ fontSize: '0.65rem', color: 'var(--text-muted)', fontWeight: 500 }}>CPU</span>
            <span style={{ fontSize: '1rem', fontWeight: 800 }}>38%</span>
            <div style={{ width: '100%', height: '4px', background: 'rgba(255, 255, 255, 0.1)', borderRadius: '99px', overflow: 'hidden' }}>
              <div style={{ width: '38%', height: '100%', background: 'var(--accent)', borderRadius: '99px' }} />
            </div>
          </div>
          <div style={{ background: 'rgba(255, 255, 255, 0.03)', border: '1px solid rgba(255, 255, 255, 0.05)', borderRadius: '8px', padding: '0.6rem', display: 'flex', flexDirection: 'column', gap: '0.2rem' }}>
            <span style={{ fontSize: '0.65rem', color: 'var(--text-muted)', fontWeight: 500 }}>RAM</span>
            <span style={{ fontSize: '1rem', fontWeight: 800 }}>54%</span>
            <div style={{ width: '100%', height: '4px', background: 'rgba(255, 255, 255, 0.1)', borderRadius: '99px', overflow: 'hidden' }}>
              <div style={{ width: '54%', height: '100%', background: 'var(--violet-primary)', borderRadius: '99px' }} />
            </div>
          </div>
          <div style={{ background: 'rgba(255, 255, 255, 0.03)', border: '1px solid rgba(255, 255, 255, 0.05)', borderRadius: '8px', padding: '0.6rem', display: 'flex', flexDirection: 'column', gap: '0.2rem' }}>
            <span style={{ fontSize: '0.65rem', color: 'var(--text-muted)', fontWeight: 500 }}>GPU</span>
            <span style={{ fontSize: '1rem', fontWeight: 800 }}>22%</span>
            <div style={{ width: '100%', height: '4px', background: 'rgba(255, 255, 255, 0.1)', borderRadius: '99px', overflow: 'hidden' }}>
              <div style={{ width: '22%', height: '100%', background: 'var(--cyan-accent)', borderRadius: '99px' }} />
            </div>
          </div>
        </div>
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', height: '100%', padding: '0 1rem', fontSize: '0.75rem' }}>
      <span>⚙️ System</span>
      <span style={{ opacity: 0.2 }}>|</span>
      <span>CPU 38%</span>
    </div>
  );
}
