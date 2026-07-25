import React from 'react';
import styles from './Simulator.module.css';
import { IslandSimulator } from '../island/IslandSimulator';

export function Simulator() {
  return (
    <section id="simulator" class={styles.section}>
      <div class={styles.header}>
        <h2>
          Experience The <span class="rose-gradient-text">Divine Island</span> Live
        </h2>
        <p>Click the mode buttons to morph the island in real time or adjust scale & opacity controls.</p>
      </div>

      <IslandSimulator />
    </section>
  );
}
