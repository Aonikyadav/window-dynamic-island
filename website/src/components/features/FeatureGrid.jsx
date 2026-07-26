import React from 'react';
import styles from './FeatureGrid.module.css';
import { featuresList } from '../../data/featuresData';

export function FeatureGrid() {
  return (
    <section id="features" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Forged in <span class={styles.roseGradient}>Performance & Precision</span>
        </h2>
        <p>Built natively for Windows with zero lag, low resource usage, and smooth animations.</p>
      </div>

      <div class={styles.grid}>
        {featuresList.map((f) => (
          <div key={f.id} class={styles.card}>
            <div class={styles.icon}>
              <i class={`fa-solid ${f.icon}`} />
            </div>
            <h3 class={styles.cardTitle}>{f.title}</h3>
            <p class={styles.cardDesc}>{f.desc}</p>
          </div>
        ))}
      </div>
    </section>
  );
}
