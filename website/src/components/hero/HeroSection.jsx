import React from 'react';
import styles from './HeroSection.module.css';

export function HeroSection() {
  return (
    <section class={styles.hero}>
      <div class={styles.badge}>
        <i class="fa-solid fa-fire-flame-curved" /> Dynamic Island for Windows
      </div>

      <h1 class={styles.title}>
        Your Windows Desktop, <br />
        <span class={styles.gradientText}>Reimagined in Divinity.</span>
      </h1>

      <p class={styles.subtitle}>
        A sleek, 60 FPS hardware-accelerated top display island bringing notifications, media playback, speech dictation, weather forecasts, and system metrics directly to your Windows desktop.
      </p>

      <div class={styles.actions}>
        <a
          href="https://github.com/Aonikyadav/window-dynamic-island/releases/latest"
          target="_blank"
          rel="noreferrer"
          class={styles.btnPrimary}
        >
          <i class="fa-solid fa-download" /> Download Executable (EXE)
        </a>
        <a
          href="https://github.com/Aonikyadav/window-dynamic-island"
          target="_blank"
          rel="noreferrer"
          class={styles.btnSecondary}
        >
          <i class="fab fa-github" /> View on GitHub
        </a>
      </div>

      <div class={styles.statsRow}>
        <div class={styles.statItem}>
          <span class={styles.statNumber}>60 FPS</span>
          <span class={styles.statLabel}>Direct2D Hardware Engine</span>
        </div>
        <div class={styles.statDivider} />
        <div class={styles.statItem}>
          <span class={styles.statNumber}>&lt; 0.1%</span>
          <span class={styles.statLabel}>CPU Background Usage</span>
        </div>
        <div class={styles.statDivider} />
        <div class={styles.statItem}>
          <span class={styles.statNumber}>OLED Black</span>
          <span class={styles.statLabel}>Custom Accent & Opacity</span>
        </div>
      </div>

      {/* Featured Desktop Screenshot (previews/home.png) */}
      <div class={styles.frameContainer}>
        <div class={styles.frameInner}>
          <img
            src="/assets/home.png"
            alt="Dynamic Island for Windows Desktop Home Preview"
            class={styles.heroImage}
          />
        </div>
      </div>
    </section>
  );
}
