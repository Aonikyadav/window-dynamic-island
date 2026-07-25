import React from 'react';
import styles from './ControlPanel.module.css';
import { modesConfig, themeColors } from '../../data/modes';

export function ControlPanel({
  currentMode,
  setCurrentMode,
  sizeScale,
  setSizeScale,
  pillOpacity,
  setPillOpacity,
  activeThemeColor,
  setActiveThemeColor
}) {
  return (
    <div class={styles.controls}>
      <h3 class={styles.title}>Select Widget Mode</h3>

      <div class={styles.grid}>
        {Object.values(modesConfig).map((mode) => {
          const isActive = currentMode === mode.id;
          return (
            <button
              key={mode.id}
              class={`${styles.ctrlBtn} ${isActive ? styles.ctrlBtnActive : ''}`}
              onClick={() => setCurrentMode(mode.id)}
            >
              <i class={`fa-solid ${mode.icon}`} />
              <span>{mode.label}</span>
            </button>
          );
        })}
      </div>

      <div class={styles.options}>
        <h4 class={styles.optionsTitle}>Style Settings</h4>

        <div class={styles.row}>
          <label>Size Scale</label>
          <input
            type="range"
            min="0.8"
            max="1.5"
            step="0.1"
            value={sizeScale}
            onChange={(e) => setSizeScale(parseFloat(e.target.value))}
          />
          <span class={styles.sliderVal}>{sizeScale.toFixed(1)}x</span>
        </div>

        <div class={styles.row}>
          <label>Pill Opacity</label>
          <input
            type="range"
            min="0.5"
            max="1.0"
            step="0.05"
            value={pillOpacity}
            onChange={(e) => setPillOpacity(parseFloat(e.target.value))}
          />
          <span class={styles.sliderVal}>{Math.round(pillOpacity * 100)}%</span>
        </div>

        <div class={styles.row}>
          <label>Theme Accent</label>
          <div class={styles.themeGrid}>
            {themeColors.map((t) => (
              <span
                key={t.color}
                class={`${styles.themeDot} ${activeThemeColor === t.color ? styles.themeDotActive : ''}`}
                style={{ background: t.color }}
                title={t.name}
                onClick={() => {
                  setActiveThemeColor(t.color);
                  document.documentElement.style.setProperty('--accent', t.color);
                  document.documentElement.style.setProperty('--accent-glow', t.color + '70');
                }}
              />
            ))}
          </div>
        </div>
      </div>
    </div>
  );
}
