import React from 'react';
import styles from './NeonGrid.module.css';

export function NeonGrid() {
  return (
    <div class={styles.gridContainer}>
      <div class={styles.perspectiveGrid} />
    </div>
  );
}
