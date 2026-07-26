import React from 'react';
import styles from './SideRail.module.css';

export function SideRail() {
  return (
    <aside class={styles.rail}>
      <div class={styles.topGridIcon}>
        <i class="fa-solid fa-table-cells-large" />
      </div>

      <div class={styles.centerSocials}>
        <span class={styles.followText}>FOLLOW US</span>
        <a href="https://github.com/Aonikyadav/window-dynamic-island" target="_blank" rel="noreferrer" class={styles.socialIcon} title="GitHub">
          <i class="fab fa-github" />
        </a>
        <a href="#" class={styles.socialIcon} title="Twitter / X">
          <i class="fab fa-x-twitter" />
        </a>
        <a href="#" class={styles.socialIcon} title="Discord">
          <i class="fab fa-discord" />
        </a>
        <a href="#" class={styles.socialIcon} title="YouTube">
          <i class="fab fa-youtube" />
        </a>
      </div>

      <div class={styles.scrollDown}>
        <span class={styles.scrollText}>SCROLL</span>
        <div class={styles.scrollLine} />
      </div>
    </aside>
  );
}
