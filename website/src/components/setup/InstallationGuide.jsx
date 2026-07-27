import React, { useState } from 'react';
import { motion } from 'framer-motion';
import styles from './InstallationGuide.module.css';

export function InstallationGuide({ onOpenDownload }) {
  const [activeTab, setActiveTab] = useState('exe');

  return (
    <section id="setup" class={styles.section}>
      <div class={styles.wrapper}>
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true }}
          transition={{ duration: 0.6 }}
          class={styles.header}
        >
          <h2>
            Simple & Instant <span class={styles.roseGradient}>Setup</span>
          </h2>
          <p>Choose your preferred installation method to get started.</p>
        </motion.div>

        <div class={styles.tabs}>
          <button
            class={`${styles.tabBtn} ${activeTab === 'exe' ? styles.tabBtnActive : ''}`}
            onClick={() => setActiveTab('exe')}
          >
            Standalone (EXE)
          </button>
          <button
            class={`${styles.tabBtn} ${activeTab === 'windhawk' ? styles.tabBtnActive : ''}`}
            onClick={() => setActiveTab('windhawk')}
          >
            Windhawk Mod
          </button>
        </div>

        {activeTab === 'exe' ? (
          <motion.div
            key="exe"
            initial={{ opacity: 0, scale: 0.96 }}
            animate={{ opacity: 1, scale: 1 }}
            transition={{ duration: 0.3 }}
            class={styles.content}
          >
            <h3>Method 1: Direct Executable</h3>
            <p>Download the pre-compiled standalone release. Zero installation or registry changes needed.</p>

            <div class={styles.stepList}>
              <div class={styles.step}>
                <div class={styles.stepNum}>1</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Download: </strong>
                  <button
                    onClick={onOpenDownload}
                    style={{
                      background: 'none',
                      border: 'none',
                      color: 'var(--rose-primary)',
                      fontWeight: 700,
                      cursor: 'pointer',
                      fontSize: '1rem',
                      textDecoration: 'underline'
                    }}
                  >
                    DynamicIsland.exe (v1.0.0)
                  </button>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>2</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Launch: </strong>
                  <span>Double-click to run. The island automatically mounts to top-center of your display.</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>3</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Configure: </strong>
                  <span>Right-click the island or click the gear icon to launch native settings, toggle auto-start, set custom wake words, and opacity.</span>
                </div>
              </div>
            </div>
          </motion.div>
        ) : (
          <motion.div
            key="windhawk"
            initial={{ opacity: 0, scale: 0.96 }}
            animate={{ opacity: 1, scale: 1 }}
            transition={{ duration: 0.3 }}
            class={styles.content}
          >
            <h3>Method 2: Install via Windhawk</h3>
            <p>Inject Dynamic Island directly into Windows Explorer using the Windhawk Mod Manager.</p>

            <div class={styles.stepList}>
              <div class={styles.step}>
                <div class={styles.stepNum}>1</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Open Windhawk: </strong>
                  <span>Launch Windhawk and click "Developer Writing Mod".</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>2</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Paste Source: </strong>
                  <span>Copy C++ source code from <a href="https://github.com/Aonikyadav/window-dynamic-island/blob/main/src/window-dynamic-island.wh.cpp" target="_blank" rel="noreferrer" style={{ color: 'var(--rose-primary)' }}><code>window-dynamic-island.wh.cpp</code></a>.</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>3</div>
                <div>
                  <strong style={{ color: 'var(--text-bright)' }}>Compile & Inject: </strong>
                  <span>Click "Compile and Inject" to run natively inside Explorer.</span>
                </div>
              </div>
            </div>
          </motion.div>
        )}
      </div>
    </section>
  );
}
