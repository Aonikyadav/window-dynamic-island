import React from 'react';
import styles from './Features.module.css';
import { featuresData } from '../../data/features';
import { NeonBadge } from '../ui/NeonBadge';

export function Features() {
  return (
    <section id="features" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Forged in <span class="rose-gradient-text">Darkness & Divinity</span>
        </h2>
        <p>Every feature is engineered for 60 FPS fluidity, zero lag, and peak aesthetic excellence.</p>
      </div>

      <div class={styles.grid}>
        {featuresData.map((f) => (
          <div key={f.id} class={styles.card}>
            <div class={styles.icon}>
              <i class={`fa-solid ${f.icon}`} />
            </div>
            <h3 class={styles.cardTitle}>{f.title}</h3>
            <p class={styles.cardDesc}>{f.description}</p>
          </div>
        ))}
      </div>
    </section>
  );
}
