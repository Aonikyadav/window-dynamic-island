import React from 'react';

export function ClipboardMode({ isExpanded }) {
  if (isExpanded) {
    return (
      <div style={{ display: 'grid', gridTemplateColumns: 'auto 1fr', gap: '1rem', padding: '1.2rem', width: '100%', height: '100%' }}>
        <div style={{ fontSize: '1.8rem', color: 'var(--accent)', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
          <i class="fa-solid fa-paste" />
        </div>
        <div style={{ display: 'flex', flexDirection: 'column', justifyContent: 'center' }}>
          <span style={{ fontSize: '0.7rem', color: 'var(--text-muted)', fontWeight: 600, textTransform: 'uppercase' }}>Clipboard Preview</span>
          <span style={{ fontSize: '0.8rem', fontWeight: 700 }}>github.com/Aonikyadav/window...</span>
        </div>
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'center', gap: '0.5rem', width: '100%', height: '100%', padding: '0 1rem', fontSize: '0.75rem', fontWeight: 600 }}>
      <i class="fa-solid fa-clipboard" />
      <span>Copied!</span>
    </div>
  );
}
