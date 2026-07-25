import React from 'react';
import styles from './Header.module.css';

export function Header() {
  return (
    <header class={styles.header}>
      <div class={styles.container}>
        <a href="#" class={styles.logo}>
          <span class={styles.iconIsland}>🏝️</span>
          <span>Dynamic<span class={styles.roseText}>Island</span></span>
        </a>
        <nav class={styles.nav}>
          <a href="#features" class={styles.navLink}>Features</a>
          <a href="#simulator" class={styles.navLink}>Live Simulator</a>
          <a href="#setup" class={styles.navLink}>Setup Guide</a>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island"
            target="_blank"
            rel="noopener noreferrer"
            class={styles.githubBtn}
          >
            <i class="fab fa-github" /> GitHub
          </a>
        </nav>
      </div>
    </header>
  );
}
