import React from 'react';
import styles from './RadarScanner.module.css';

export function RadarScanner() {
  return (
    <div class={styles.radarContainer}>
      <div class={styles.radarGrid} />
      <div class={styles.radarSweep} />
      <div class={styles.radarCenter} />
    </div>
  );
}
