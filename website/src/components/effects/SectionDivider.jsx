import React from 'react';
import styles from './SectionDivider.module.css';

export function SectionDivider() {
  return (
    <div class={styles.dividerContainer}>
      <div class={styles.line} />
      <div class={styles.centerDiamond} />
    </div>
  );
}
