import React from 'react';
import styles from './Footer.module.css';

export function Footer({ onOpenDownload }) {
  return (
    <footer class={styles.footer}>
      <div class={styles.container}>
        <div class={styles.brand}>
          <div class={styles.logo}>
            <span>🏝️ Dynamic<span class={styles.roseText}>Island</span></span>
          </div>
          <p class={styles.desc}>
            A hardware-accelerated, 60 FPS Dynamic Island overlay for Windows 10 & 11 desktops built with Win32, Direct2D, and WinRT APIs.
          </p>
        </div>

        <div class={styles.links}>
          <h4 class={styles.linksTitle}>Quick Links</h4>
          <a href="https://github.com/Aonikyadav" target="_blank" rel="noreferrer" class={styles.link}>
            <i class="fa-brands fa-github" /> Official GitHub Profile (@Aonikyadav)
          </a>
          <button onClick={onOpenDownload} class={styles.link} style={{ background: 'none', border: 'none', padding: 0, textAlign: 'left', cursor: 'pointer' }}>
            <i class="fa-solid fa-download" /> Download Standalone (EXE)
          </button>
          <a href="https://github.com/Aonikyadav" target="_blank" rel="noreferrer" class={styles.link}>
            <i class="fa-solid fa-user" /> Official GitHub Profile
          </a>
        </div>

        <div class={styles.credits}>
          <p>© {new Date().getFullYear()} Aonik Yadav. Distributed under the MIT License.</p>
        </div>
      </div>
    </footer>
  );
}
