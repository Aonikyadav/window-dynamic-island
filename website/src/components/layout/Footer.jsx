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
          <p class={styles.desc}>
            A hardware-accelerated, 60 FPS Dynamic Island overlay for Windows desktops built with Win32, Direct2D, and WinRT APIs.
          </p>
        </div>

        <div class={styles.links}>
          <h4 class={styles.linksTitle}>Navigation</h4>
          <a href="https://github.com/Aonikyadav/window-dynamic-island" target="_blank" rel="noreferrer" class={styles.link}>
            GitHub Repository
          </a>
          <a href="https://github.com/Aonikyadav/window-dynamic-island/releases" target="_blank" rel="noreferrer" class={styles.link}>
            Latest Releases (EXE)
          </a>
          <a href="https://github.com/Aonikyadav/window-dynamic-island/issues" target="_blank" rel="noreferrer" class={styles.link}>
            Report an Issue
          </a>
        </div>

        <div class={styles.credits}>
          <p>© {new Date().getFullYear()} Aonik Yadav. Distributed under the MIT License.</p>
        </div>
      </div>
    </footer>
  );
}
