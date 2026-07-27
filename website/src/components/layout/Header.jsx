import React from 'react';
import styles from './Header.module.css';

export function Header({ onOpenDownload }) {
  return (
    <header class={styles.header}>
      <div class={styles.container}>
        <a href="#" class={styles.logo}>
          <i class={`fa-solid fa-square-xmark ${styles.logoIcon}`} />
          <span>DYNAMIC<span class={styles.roseText}>ISLAND</span></span>
        </a>

        <nav class={styles.nav}>
          <a href="#" class={`${styles.navLink} ${styles.navLinkActive}`}>HOME</a>
          <a href="#simulator" class={styles.navLink}>SHOWCASE</a>
          <a href="#ai" class={styles.navLink}>VOICE AI</a>
          <a href="#features" class={styles.navLink}>FEATURES</a>
          <a href="#setup" class={styles.navLink}>DOWNLOAD & SETUP</a>
        </nav>

        <div class={styles.rightGroup}>
          <a
            href="https://github.com/Aonikyadav"
            target="_blank"
            rel="noreferrer"
            class={styles.searchIcon}
            title="GitHub Profile"
          >
            <i class="fa-brands fa-github" style={{ fontSize: '1.2rem' }} />
          </a>
          <button
            onClick={onOpenDownload}
            class={styles.ctaBtn}
          >
            <span>DOWNLOAD</span>
            <i class="fa-solid fa-arrow-down-long" />
          </button>
        </div>
      </div>
    </header>
  );
}
