import React from 'react';
import styles from './Screenshots.module.css';

export function Screenshots() {
  const slots = [
    { title: 'STT Live Transcribing', icon: 'fa-microphone' },
    { title: 'Media Waveform Player', icon: 'fa-music' },
    { title: 'System Performance Grid', icon: 'fa-microchip' }
  ];

  return (
    <section class={styles.section}>
      <div class={styles.header}>
        <h2>
          Desktop Preview <span class="rose-gradient-text">Gallery</span>
        </h2>
        <p>High-resolution screenshots of Dynamic Island running natively on Windows desktop.</p>
      </div>

      <div class={styles.grid}>
        {slots.map((s, idx) => (
          <div key={idx} class={styles.slot}>
            <i class={`fa-solid ${s.icon} ${styles.slotIcon}`} />
            <span class={styles.slotText}>{s.title}</span>
          </div>
        ))}
      </div>
    </section>
  );
}
