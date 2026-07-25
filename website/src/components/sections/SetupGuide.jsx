import React, { useState } from 'react';
import styles from './SetupGuide.module.css';
import { standaloneSteps, windhawkSteps, windhawkCodeSnippet } from '../../data/setupSteps';
import { CodeBox } from '../ui/CodeBox';

export function SetupGuide() {
  const [activeTab, setActiveTab] = useState('standalone');

  return (
    <section id="setup" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Command <span class="rose-gradient-text">The Divine Island</span>
        </h2>
        <p>Choose your preferred installation method below.</p>
      </div>

      <div class={styles.wrapper}>
        <div class={styles.tabs}>
          <button
            class={`${styles.tabBtn} ${activeTab === 'standalone' ? styles.tabBtnActive : ''}`}
            onClick={() => setActiveTab('standalone')}
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

        {activeTab === 'standalone' ? (
          <div class={styles.content}>
            <h3>Method 1: Direct Executable</h3>
            <p>Download the pre-compiled standalone version of Dynamic Island, which runs instantly.</p>

            <div class={styles.stepList}>
              {standaloneSteps.map((s) => (
                <div key={s.step} class={styles.step}>
                  <div class={styles.stepNum}>{s.step}</div>
                  <div>
                    <strong>{s.title}: </strong>
                    <span style={{ color: 'var(--text-muted)' }}>{s.desc}</span>
                  </div>
                </div>
              ))}
            </div>
          </div>
        ) : (
          <div class={styles.content}>
            <h3>Method 2: Install via Windhawk</h3>
            <p>If you use Windhawk mod manager, inject Dynamic Island directly into Windows Shell.</p>

            <CodeBox code={windhawkCodeSnippet} />

            <div class={styles.stepList}>
              {windhawkSteps.map((s) => (
                <div key={s.step} class={styles.step}>
                  <div class={styles.stepNum}>{s.step}</div>
                  <div>
                    <strong>{s.title}: </strong>
                    <span style={{ color: 'var(--text-muted)' }}>{s.desc}</span>
                  </div>
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    </section>
  );
}
