import React, { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import styles from './HeroSection.module.css';
import { RadarScanner } from '../effects/RadarScanner';

export function HeroSection() {
  const [activePillIndex, setActivePillIndex] = useState(0);

  const pillModes = [
    {
      id: 'media',
      content: (
        <>
          <div class={styles.spinnerArt} />
          <span>Super Saiyan Rosé — Theme</span>
          <div class={styles.waveform}>
            <span /><span /><span />
          </div>
        </>
      )
    },
    {
      id: 'stt',
      content: (
        <>
          <i class="fa-solid fa-microphone" style={{ color: 'var(--rose-primary)' }} />
          <span>Listening Dictation...</span>
          <span style={{ color: 'var(--rose-primary)', fontWeight: 800 }}>LIVE</span>
        </>
      )
    },
    {
      id: 'system',
      content: (
        <>
          <span>⚙️ Direct2D Engine</span>
          <span style={{ opacity: 0.4 }}>|</span>
          <span style={{ color: '#4ade80', fontWeight: 800 }}>60 FPS Lock</span>
        </>
      )
    }
  ];

  useEffect(() => {
    const timer = setInterval(() => {
      setActivePillIndex((prev) => (prev + 1) % pillModes.length);
    }, 4000);
    return () => clearInterval(timer);
  }, [pillModes.length]);

  return (
    <section class={styles.hero}>
      {/* High-Contrast Backdrop Title (METAVERSE Style) */}
      <div class={styles.giantBgTitle}>
        DYNAMIC ISLAND
      </div>

      {/* Seamless Background-Removed Character Centerpiece (home.png) */}
      <div class={styles.heroVisualCenter}>
        {/* Animated Morphing Pill Floating Above Character */}
        <AnimatePresence mode="wait">
          <motion.div
            key={pillModes[activePillIndex].id}
            initial={{ opacity: 0, scale: 0.85, y: -5 }}
            animate={{ opacity: 1, scale: 1, y: 0 }}
            exit={{ opacity: 0, scale: 0.85, y: 5 }}
            transition={{ type: 'spring', stiffness: 260, damping: 20 }}
            class={styles.heroIslandPill}
          >
            {pillModes[activePillIndex].content}
          </motion.div>
        </AnimatePresence>

        <motion.img
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ duration: 0.8 }}
          src="/assets/home.png"
          alt="Dynamic Island Goku Black Rosé Character Cutout"
          class={styles.heroCutoutImg}
        />
      </div>

      {/* Dual Column High-Contrast Content */}
      <div class={styles.dualColumns}>
        {/* Left Column */}
        <motion.div
          initial={{ opacity: 0, x: -30 }}
          animate={{ opacity: 1, x: 0 }}
          transition={{ duration: 0.7, delay: 0.2 }}
          class={styles.leftCol}
        >
          <span class={styles.eyebrow}>ENTER THE NEXT DIMENSION</span>
          <h1 class={styles.colTitle}>
            DYNAMIC <span class={styles.roseText}>OVERLAY</span>
          </h1>
          <p class={styles.colDesc}>
            A fluid, hardware-accelerated top display island bringing notifications, media waveforms, speech dictation, weather forecasts, and system metrics directly to your Windows desktop.
          </p>
          <a href="#simulator" class={styles.btnSlanted}>
            <span>EXPLORE LIVE SIMULATOR</span>
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
          <h2 class={styles.colTitle}>
            THE <span class={styles.roseText}>FUTURE</span>
          </h2>
          <p class={styles.colDesc}>
            Engineered natively with Win32, Direct2D, and WinRT APIs for 60 FPS liquid physics, dual-spring velocity interpolation, and near-zero background CPU impact.
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

      {/* Floating Glassmorphic HUD Bar (Bottom - Fixed Unclipped Alignment) */}
      <motion.div
        initial={{ opacity: 0, y: 40 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.8, delay: 0.4 }}
        class={styles.hudBar}
      >
        <div style={{ display: 'flex', alignItems: 'center', gap: '1.2rem', flex: 1 }}>
          <RadarScanner />
          <div class={styles.hudMetrics}>
            <div class={styles.hudMetricBlock}>
              <span class={styles.hudMetricTitle}>ACTIVE USERS</span>
              <span class={styles.hudMetricVal}>+15K <span style={{ fontSize: '0.8rem', color: 'var(--text-bright)' }}>DL</span></span>
              <span class={styles.hudMetricSub}>Active desktop installations</span>
            </div>

            <div class={styles.hudMetricBlock}>
              <span class={styles.hudMetricTitle}>PERFORMANCE</span>
              <span class={styles.hudMetricVal}>60 FPS</span>
              <span class={styles.hudMetricSub}>Direct2D with &lt; 0.1% CPU</span>
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

        <a href="#simulator" class={styles.hudActionBtn} title="Try Live Simulator">
          <i class="fa-solid fa-arrow-right" />
        </a>
      </motion.div>
    </section>
  );
}
