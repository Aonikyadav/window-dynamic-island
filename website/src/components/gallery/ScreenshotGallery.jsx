import React from 'react';
import { motion } from 'framer-motion';
import styles from './ScreenshotGallery.module.css';
import { galleryScreenshots } from '../../data/galleryData';

export function ScreenshotGallery() {
  return (
    <section id="gallery" class={styles.section}>
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        whileInView={{ opacity: 1, y: 0 }}
        viewport={{ once: true }}
        transition={{ duration: 0.6 }}
        class={styles.header}
      >
        <h2>
          Desktop Preview <span class={styles.roseGradient}>Gallery</span>
        </h2>
        <p>High-resolution captures of Dynamic Island operating live on Windows displays.</p>
      </motion.div>

      <div class={styles.grid}>
        {galleryScreenshots.map((item, idx) => (
          <motion.div
            key={idx}
            initial={{ opacity: 0, y: 30 }}
            whileInView={{ opacity: 1, y: 0 }}
            viewport={{ once: true }}
            transition={{ duration: 0.6, delay: idx * 0.15 }}
            class={styles.item}
          >
            <img src={item.image} alt={item.title} class={styles.image} />
            <div class={styles.caption}>
              <span class={styles.itemTitle}>{item.title}</span>
              <span class={styles.itemSub}>{item.subtitle}</span>
            </div>
          </motion.div>
        ))}
      </div>
    </section>
  );
}
