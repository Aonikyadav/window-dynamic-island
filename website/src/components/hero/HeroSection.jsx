import React from 'react';
import { motion } from 'framer-motion';
import styles from './HeroSection.module.css';
import { RadarScanner } from '../effects/RadarScanner';

export function HeroSection() {
  return (
    <section class={styles.hero}>
      {/* Giant Distressed Background Title (METAVERSE Style) */}
      <div class={styles.giantBgTitle}>
        DYNAMIC ISLAND
      </div>

      {/* Centerpiece Visual Mockup (home.png) */}
      <div class={styles.heroVisualCenter}>
        <motion.div
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ duration: 0.8 }}
          class={styles.heroImageContainer}
        >
          <img
            src="/assets/home.png"
            alt="Dynamic Island for Windows Home Preview"
            class={styles.heroImage}
          />
        </motion.div>
      </div>

      {/* Dual Column Text & CTA Content */}
      <div class={styles.dualColumns}>
        {/* Left Column */}
        <motion.div
          initial={{ opacity: 0, x: -30 }}
          animate={{ opacity: 1, x: 0 }}
          transition={{ duration: 0.7, delay: 0.2 }}
          class={styles.leftCol}
        >
          <span class={styles.eyebrow}>ENTER THE NEXT DIMENSION</span>
          <h2 class={styles.colTitle}>DYNAMIC OVERLAY</h2>
          <p class={styles.colDesc}>
            A fluid, hardware-accelerated top display island bringing notifications, media playback, speech dictation, weather forecasts, and system metrics directly to your Windows desktop.
          </p>
          <a
            href="#showcase"
            class={styles.btnSlanted}
          >
            <span>EXPLORE NOW</span>
            <i class="fa-solid fa-angles-right" />
          </a>
        </motion.div>

        {/* Right Column */}
        <motion.div
          initial={{ opacity: 0, x: 30 }}
          animate={{ opacity: 1, x: 0 }}
          transition={{ duration: 0.7, delay: 0.3 }}
          class={styles.rightCol}
        >
          <div class={styles.tagRow}>
            <span>WHERE IMAGINATION MEETS REALITY</span>
            <span class={styles.tagNum}>01 / 05</span>
          </div>
          <h2 class={styles.colTitle}>THE FUTURE</h2>
          <p class={styles.colDesc}>
            Engineered natively with Win32, Direct2D, and WinRT APIs for 60 FPS smoothness, dual-spring physics transitions, and near-zero background CPU impact.
          </p>
          <a
            href="https://github.com/Aonikyadav/window-dynamic-island/releases/latest"
            target="_blank"
            rel="noreferrer"
            class={styles.btnCircle}
          >
            <i class="fa-solid fa-download" />
            <span>DOWNLOAD EXE</span>
          </a>
        </motion.div>
      </div>

      {/* Floating Glassmorphic HUD Bar (Bottom) */}
      <motion.div
        initial={{ opacity: 0, y: 40 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.8, delay: 0.4 }}
        class={styles.hudBar}
      >
        <div style={{ display: 'flex', alignItems: 'center', gap: '1.5rem' }}>
          <RadarScanner />
          <div class={styles.hudMetrics}>
            <div class={styles.hudMetricBlock}>
              <span class={styles.hudMetricTitle}>ACTIVE USERS</span>
              <span class={styles.hudMetricVal}>+15K <span style={{ fontSize: '0.8rem', color: 'var(--text-bright)' }}>DL</span></span>
              <span class={styles.hudMetricSub}>Active desktop installations worldwide</span>
            </div>

            <div class={styles.hudMetricBlock}>
              <span class={styles.hudMetricTitle}>PERFORMANCE</span>
              <span class={styles.hudMetricVal}>60 FPS</span>
              <span class={styles.hudMetricSub}>Direct2D hardware engine with &lt; 0.1% CPU</span>
            </div>

            <div class={styles.hudMetricBlock}>
              <span class={styles.hudMetricTitle}>GLOBAL COMMUNITY</span>
              <span class={styles.hudMetricVal} style={{ fontSize: '1rem', color: 'var(--text-bright)' }}>
                <i class="fab fa-github" style={{ color: 'var(--rose-primary)' }} /> Open Source
              </span>
              <span class={styles.hudMetricSub}>MIT License on GitHub</span>
            </div>
          </div>
        </div>

        <a href="#showcase" class={styles.hudActionBtn} title="Explore Showcase">
          <i class="fa-solid fa-arrow-right" />
        </a>
      </motion.div>
    </section>
  );
}
