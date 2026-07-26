import React, { useState } from 'react';
import styles from './LiveShowcase.module.css';
import { showcaseTabs } from '../../data/showcaseData';

export function LiveShowcase() {
  const [activeTabId, setActiveTabId] = useState('hero');

  const activeTab = showcaseTabs.find((t) => t.id === activeTabId) || showcaseTabs[0];

  return (
    <section id="showcase" class={styles.section}>
      <div class={styles.container}>
        <div class={styles.header}>
          <h2>
            Explore The <span class={styles.roseGradient}>Dynamic Features</span>
          </h2>
          <p>Click any category tab to inspect the actual desktop screenshot preview and capabilities.</p>
        </div>

        {/* Tab Selector Bar */}
        <div class={styles.tabBar}>
          {showcaseTabs.map((tab) => (
            <button
              key={tab.id}
              class={`${styles.tabBtn} ${activeTabId === tab.id ? styles.tabBtnActive : ''}`}
              onClick={() => setActiveTabId(tab.id)}
            >
              <i class={`fa-solid ${tab.icon}`} />
              <span>{tab.label}</span>
            </button>
          ))}
        </div>

        {/* Dynamic Display Panel */}
        <div class={styles.displayGrid}>
          <div class={styles.infoBox}>
            <span class={styles.badge}>{activeTab.badge}</span>
            <h3 class={styles.infoTitle}>{activeTab.title}</h3>
            <p class={styles.infoDesc}>{activeTab.description}</p>
          </div>

          <div class={styles.previewFrame}>
            <img
              src={activeTab.image}
              alt={activeTab.title}
              class={styles.previewImg}
            />
          </div>
        </div>
      </div>
    </section>
  );
}
