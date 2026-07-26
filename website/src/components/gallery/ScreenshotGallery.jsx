import React from 'react';
import styles from './ScreenshotGallery.module.css';
import { galleryScreenshots } from '../../data/galleryData';

export function ScreenshotGallery() {
  return (
    <section id="gallery" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Desktop Preview <span class={styles.roseGradient}>Gallery</span>
        </h2>
        <p>High-resolution captures of Dynamic Island operating live on Windows displays.</p>
      </div>

      <div class={styles.grid}>
        {galleryScreenshots.map((item, idx) => (
          <div key={idx} class={styles.item}>
            <img src={item.image} alt={item.title} class={styles.image} />
            <div class={styles.caption}>
              <span class={styles.itemTitle}>{item.title}</span>
              <span class={styles.itemSub}>{item.subtitle}</span>
            </div>
          </div>
        ))}
      </div>
    </section>
  );
}
