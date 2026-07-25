import React from 'react';
import styles from './StatCard.module.css';

export function StatCard({ number, label }) {
  return (
    <div class={styles.statItem}>
      <span class={styles.number}>{number}</span>
      <span class={styles.label}>{label}</span>
    </div>
  );
}
