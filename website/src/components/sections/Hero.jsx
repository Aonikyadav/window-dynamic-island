import React from 'react';
import styles from './Hero.module.css';
import { NeonBadge } from '../ui/NeonBadge';
import { GlowButton } from '../ui/GlowButton';

export function Hero() {
  return (
    <section class={styles.hero}>
      <div class={styles.content}>
        <NeonBadge>Super Saiyan Rosé Edition</NeonBadge>
        <h1 class={styles.title}>
          Divine Power. <br />
          Meet your <span class={styles.roseGradientText}>Windows Desktop</span>.
        </h1>
        <p class={styles.subtitle}>
          A gorgeous, hardware-accelerated, highly interactive desktop overlay inspired by Apple's Dynamic Island, styled with the glowing magenta-violet aura of Goku Black.
        </p>
        <div class={styles.actions}>
          <GlowButton
            href="https://github.com/Aonikyadav"
            target="_blank"
            rel="noreferrer"
            variant="primary"
            icon="fa-brands fa-github"
          >
            Official GitHub Profile
          </GlowButton>
          <GlowButton href="#setup" variant="secondary" icon="fa-solid fa-terminal">
            Quick Install
          </GlowButton>
        </div>
      </div>

      <div class={styles.visual}>
        <div class={styles.previewIslandWrapper}>
          <div class={styles.previewIslandShadow} />
          <div class={styles.previewIslandBorder} />
          <div class={styles.previewIslandBase}>
            <span>10:09 PM</span>
            <span style={{ opacity: 0.2 }}>|</span>
            <span style={{ display: 'flex', alignItems: 'center', gap: '0.4rem' }}>
              <i class="fa-solid fa-desktop" style={{ color: 'var(--rose-primary)' }} /> window-dynamic-island
            </span>
          </div>
        </div>
      </div>
    </section>
  );
}
