import React from 'react';
import styles from './Header.module.css';

export function Header() {
  return (
    <header class={styles.header}>
      <div class={styles.container}>
        <a href="#" class={styles.logo}>
          <i class={`fa-solid fa-square-xmark ${styles.logoIcon}`} />
          <span>DYNAMIC<span class={styles.roseText}>ISLAND</span></span>
        </a>

        <nav class={styles.nav}>
          <a href="#" class={`${styles.navLink} ${styles.navLinkActive}`}>HOME</a>
          <a href="#showcase" class={styles.navLink}>SHOWCASE</a>
          <a href="#features" class={styles.navLink}>FEATURES</a>
          <a href="#gallery" class={styles.navLink}>GALLERY</a>
          <a href="#setup" class={styles.navLink}>SUPPORT</a>
        </nav>

        <div class={styles.rightGroup}>
          <div class={styles.searchIcon} title="Search">
            <i class="fa-solid fa-magnifying-glass" />
          </div>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island/releases/latest"
            target="_blank"
            rel="noreferrer"
            class={styles.ctaBtn}
          >
            <span>DOWNLOAD</span>
            <i class="fa-solid fa-arrow-right" />
          </a>
        </div>
      </div>
    </header>
  );
}
