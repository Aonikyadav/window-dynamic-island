import React from 'react';
import { motion } from 'framer-motion';
import styles from './FeatureGrid.module.css';
import { featuresList } from '../../data/featuresData';

export function FeatureGrid() {
  return (
    <section id="features" class={styles.section}>
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        whileInView={{ opacity: 1, y: 0 }}
        viewport={{ once: true }}
        transition={{ duration: 0.6 }}
        class={styles.header}
      >
        <h2>
          Forged in <span class={styles.roseGradient}>Performance & Precision</span>
        </h2>
        <p>Built natively for Windows with zero lag, low resource usage, and smooth animations.</p>
      </motion.div>

      <div class={styles.grid}>
        {featuresList.map((f, idx) => (
          <motion.div
            key={f.id}
            initial={{ opacity: 0, y: 25 }}
            whileInView={{ opacity: 1, y: 0 }}
            viewport={{ once: true }}
            transition={{ duration: 0.5, delay: idx * 0.1 }}
            class={styles.card}
          >
            <div class={styles.icon}>
              <i class={`fa-solid ${f.icon}`} />
            </div>
            <h3 class={styles.cardTitle}>{f.title}</h3>
            <p class={styles.cardDesc}>{f.desc}</p>
          </motion.div>
        ))}
      </div>
    </section>
  );
}
