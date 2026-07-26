import React, { useState } from 'react';
import styles from './InstallationGuide.module.css';

export function InstallationGuide() {
  const [activeTab, setActiveTab] = useState('exe');

  return (
    <section id="setup" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Simple & Instant <span class={styles.roseGradient}>Setup</span>
        </h2>
        <p>Choose your preferred installation method to get started.</p>
      </div>

      <div class={styles.wrapper}>
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
          <div class={styles.content}>
            <h3>Method 1: Direct Executable</h3>
            <p>Download the pre-compiled standalone release. Zero installation or registry changes needed.</p>

            <div class={styles.stepList}>
              <div class={styles.step}>
                <div class={styles.stepNum}>1</div>
                <div>
                  <strong>Download: </strong>
                  <a href="https://github.com/Aonikyadav/window-dynamic-island/releases/latest" target="_blank" rel="noreferrer" style={{ color: 'var(--rose-primary)', fontWeight: 600 }}>
                    DynamicIsland.exe
                  </a>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>2</div>
                <div>
                  <strong>Launch: </strong>
                  <span style={{ color: 'var(--text-muted)' }}>Double-click to start. The island automatically appears at top-center display.</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>3</div>
                <div>
                  <strong>Configure: </strong>
                  <span style={{ color: 'var(--text-muted)' }}>Right-click the island to launch native settings and set auto-start.</span>
                </div>
              </div>
            </div>
          </div>
        ) : (
          <div class={styles.content}>
            <h3>Method 2: Install via Windhawk</h3>
            <p>Inject Dynamic Island directly into Windows Explorer using the Windhawk Mod Manager.</p>

            <div class={styles.stepList}>
              <div class={styles.step}>
                <div class={styles.stepNum}>1</div>
                <div>
                  <strong>Open Windhawk: </strong>
                  <span style={{ color: 'var(--text-muted)' }}>Launch Windhawk and click "Developer Writing Mod".</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>2</div>
                <div>
                  <strong>Paste Source: </strong>
                  <span style={{ color: 'var(--text-muted)' }}>Copy source code from <code>window-dynamic-island.wh.cpp</code>.</span>
                </div>
              </div>
              <div class={styles.step}>
                <div class={styles.stepNum}>3</div>
                <div>
                  <strong>Compile & Inject: </strong>
                  <span style={{ color: 'var(--text-muted)' }}>Click "Compile and Inject" to run natively.</span>
                </div>
              </div>
            </div>
          </div>
        )}
      </div>
    </section>
  );
}
