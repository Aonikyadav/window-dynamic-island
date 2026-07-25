import React from 'react';
import styles from './AuraBackground.module.css';

export function AuraBackground() {
  return (
    <div class={styles.auraContainer}>
      <div class={`${styles.auraGlow} ${styles.primaryGlow}`} />
      <div class={`${styles.auraGlow} ${styles.secondaryGlow}`} />
    </div>
  );
}
