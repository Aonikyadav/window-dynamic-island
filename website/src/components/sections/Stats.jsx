import React from 'react';
import styles from './Stats.module.css';
import { StatCard } from '../ui/StatCard';

export function Stats() {
  return (
    <div class={styles.container}>
      <StatCard number="60 FPS" label="Hardware-Accelerated" />
      <div class={styles.divider} />
      <StatCard number="< 0.1%" label="CPU Background Usage" />
      <div class={styles.divider} />
      <StatCard number="100%" label="OLED Black & Custom Accent" />
    </div>
  );
}
