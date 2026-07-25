import React from 'react';
import styles from './NeonBadge.module.css';

export function NeonBadge({ children, icon = 'fa-solid fa-fire-flame-curved' }) {
  return (
    <div class={styles.badge}>
      {icon && <i class={icon} />}
      {children}
    </div>
  );
}
