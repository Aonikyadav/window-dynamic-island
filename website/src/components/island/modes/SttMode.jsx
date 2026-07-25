import React from 'react';

export function SttMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{ display: 'flex', flexDirection: 'column', padding: '1.2rem', width: '100%', height: '100%', gap: '0.6rem' }}>
        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', borderBottom: '1px solid rgba(255, 255, 255, 0.05)', paddingBottom: '0.4rem' }}>
          <span style={{ fontSize: '0.75rem', fontWeight: 700 }}>Speech-to-Text</span>
          <span style={{
            background: 'rgba(255, 42, 133, 0.15)',
            border: '1px solid var(--accent)',
            color: 'var(--accent)',
            fontSize: '0.65rem',
            fontWeight: 700,
            padding: '0.1rem 0.5rem',
            borderRadius: '4px',
            animation: 'live-pulse 1.5s infinite ease-in-out'
          }}>LIVE</span>
        </div>
        <div style={{ fontSize: '0.75rem', color: 'var(--text-bright)', lineHeight: 1.4, minHeight: '52px' }}>
          Watch me transcribe speech in real time...
          <span style={{ display: 'inline-block', width: '5px', height: '11px', background: 'var(--accent)', animation: 'cursor-blink 0.8s infinite', marginLeft: '2px' }} />
        </div>
        <div style={{ display: 'flex', gap: '0.5rem', borderTop: '1px solid rgba(255, 255, 255, 0.05)', paddingTop: '0.6rem' }}>
          <button style={{ background: 'var(--accent)', border: 'none', color: '#fff', fontSize: '0.65rem', padding: '0.3rem 0.6rem', borderRadius: '4px', cursor: 'pointer', fontWeight: 600 }}>📋 Copy</button>
          <button style={{ background: 'rgba(255, 255, 255, 0.05)', border: '1px solid rgba(255, 255, 255, 0.07)', color: 'var(--text-bright)', fontSize: '0.65rem', padding: '0.3rem 0.6rem', borderRadius: '4px', cursor: 'pointer' }}>Clear</button>
          <button style={{ background: 'rgba(255, 255, 255, 0.05)', border: '1px solid rgba(255, 255, 255, 0.07)', color: 'var(--text-bright)', fontSize: '0.65rem', padding: '0.3rem 0.6rem', borderRadius: '4px', cursor: 'pointer' }}>Notepad ↗</button>
        </div>
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', height: '100%', padding: '0 1rem', fontSize: '0.75rem', fontWeight: 600 }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '0.4rem' }}>
        <i class="fa-solid fa-microphone" />
        <span>STT</span>
      </div>
      <div style={{ width: '8px', height: '8px', borderRadius: '50%', background: 'var(--rose-primary)', boxShadow: '0 0 6px var(--rose-primary)', animation: 'live-pulse 1s infinite alternate' }} />
    </div>
  );
}
