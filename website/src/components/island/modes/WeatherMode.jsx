import React from 'react';

export function WeatherMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{
        display: 'grid',
        gridTemplateColumns: '1fr 1.2fr',
        gap: '1rem',
        padding: '1.2rem',
        width: '100%',
        height: '100%'
      }}>
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          justifyContent: 'center',
          alignItems: 'center',
          borderRight: '1px solid rgba(255, 255, 255, 0.07)',
          paddingRight: '1rem'
        }}>
          <div style={{ fontSize: '2.2rem', fontWeight: 800, lineHeight: 1, marginBottom: '0.3rem' }}>24°C</div>
          <div style={{ fontSize: '0.75rem', color: 'var(--text-muted)', fontWeight: 500 }}>Mathura, IN</div>
        </div>
        <div style={{ display: 'flex', flexDirection: 'column', justifyContent: 'center', gap: '0.6rem', fontSize: '0.75rem' }}>
          <div style={{ display: 'flex', justifyContent: 'space-between', borderBottom: '1px solid rgba(255, 255, 255, 0.03)', paddingBottom: '0.3rem' }}>
            <span style={{ color: 'var(--text-muted)' }}>Wind</span>
            <span style={{ fontWeight: 600, color: 'var(--accent)' }}>E 14 km/h</span>
          </div>
          <div style={{ display: 'flex', justifyContent: 'space-between', borderBottom: '1px solid rgba(255, 255, 255, 0.03)', paddingBottom: '0.3rem' }}>
            <span style={{ color: 'var(--text-muted)' }}>Humidity</span>
            <span style={{ fontWeight: 600, color: 'var(--accent)' }}>62%</span>
          </div>
          <div style={{ display: 'flex', justifyContent: 'space-between', borderBottom: '1px solid rgba(255, 255, 255, 0.03)', paddingBottom: '0.3rem' }}>
            <span style={{ color: 'var(--text-muted)' }}>Feels Like</span>
            <span style={{ fontWeight: 600, color: 'var(--accent)' }}>26°C</span>
          </div>
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
      padding: '0 1rem',
      fontSize: '0.75rem'
    }}>
      <span style={{ display: 'flex', alignItems: 'center', gap: '0.5rem', fontWeight: 600 }}>
        <i class="fa-solid fa-cloud-sun" style={{ color: '#ffd23f' }} /> 24°C
      </span>
      <span style={{ opacity: 0.7 }}>Mathura</span>
    </div>
  );
}
