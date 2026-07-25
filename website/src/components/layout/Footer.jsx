import React from 'react';
import styles from './Footer.module.css';

export function Footer() {
  return (
    <footer class={styles.footer}>
      <div class={styles.container}>
        <div class={styles.brand}>
          <div class={styles.logo}>
            <span>🏝️ Dynamic<span class={styles.roseText}>Island</span></span>
          </div>
          <p class={styles.brandDesc}>
            A divine, hardware-accelerated desktop overlay for Windows inspired by Super Saiyan Rosé aesthetics.
          </p>
        </div>
        <div class={styles.links}>
          <h4 class={styles.linksTitle}>Project Links</h4>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island"
            target="_blank"
            rel="noopener noreferrer"
            class={styles.link}
          >
            GitHub Repository
          </a>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island/releases"
            target="_blank"
            rel="noopener noreferrer"
            class={styles.link}
          >
            Latest Releases
          </a>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island/issues"
            target="_blank"
            rel="noopener noreferrer"
            class={styles.link}
          >
            Report Issues
          </a>
        </div>
        <div class={styles.credits}>
          <p>© {new Date().getFullYear()} Aonik Yadav. Maintained with divine power. MIT License.</p>
        </div>
      </div>
    </footer>
  );
}
