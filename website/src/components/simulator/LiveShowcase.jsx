import React, { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import styles from './LiveShowcase.module.css';
import { showcaseTabs } from '../../data/showcaseData';

export function LiveShowcase() {
  const [activeTabId, setActiveTabId] = useState('hero');

  const activeTab = showcaseTabs.find((t) => t.id === activeTabId) || showcaseTabs[0];

  return (
    <section id="showcase" class={styles.section}>
      <div class={styles.container}>
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true }}
          transition={{ duration: 0.6 }}
          class={styles.header}
        >
          <h2>
            Explore The <span class={styles.roseGradient}>Dynamic Features</span>
          </h2>
          <p>Click any category tab to inspect the actual desktop screenshot preview and capabilities.</p>
        </motion.div>

        {/* Tab Selector Bar */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true }}
          transition={{ duration: 0.6, delay: 0.1 }}
          class={styles.tabBar}
        >
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
        </motion.div>

        {/* Dynamic Display Panel */}
        <motion.div
          initial={{ opacity: 0, scale: 0.96 }}
          whileInView={{ opacity: 1, scale: 1 }}
          viewport={{ once: true }}
          transition={{ duration: 0.7, delay: 0.2 }}
          class={styles.displayGrid}
        >
          <AnimatePresence mode="wait">
            <motion.div
              key={activeTab.id}
              initial={{ opacity: 0, x: -20 }}
              animate={{ opacity: 1, x: 0 }}
              exit={{ opacity: 0, x: 20 }}
              transition={{ duration: 0.3 }}
              class={styles.infoBox}
            >
              <span class={styles.badge}>{activeTab.badge}</span>
              <h3 class={styles.infoTitle}>{activeTab.title}</h3>
              <p class={styles.infoDesc}>{activeTab.description}</p>
            </motion.div>
          </AnimatePresence>

          <AnimatePresence mode="wait">
            <motion.div
              key={activeTab.id}
              initial={{ opacity: 0, scale: 0.95 }}
              animate={{ opacity: 1, scale: 1 }}
              exit={{ opacity: 0, scale: 0.95 }}
              transition={{ duration: 0.3 }}
              class={styles.previewFrame}
            >
              <img
                src={activeTab.image}
                alt={activeTab.title}
                class={styles.previewImg}
              />
            </motion.div>
          </AnimatePresence>
        </motion.div>
      </div>
    </section>
  );
}
