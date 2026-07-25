import React from 'react';
import styles from './IslandSimulator.module.css';
import { IslandPill } from './IslandPill';
import { ControlPanel } from './ControlPanel';
import { useIslandSimulator } from '../../hooks/useIslandSimulator';

export function IslandSimulator() {
  const sim = useIslandSimulator();

  return (
    <div class={styles.container}>
      <div
        class={styles.desktopScreen}
        onMouseEnter={() => sim.setIsHovered(true)}
        onMouseLeave={() => sim.setIsHovered(false)}
      >
        <div class={styles.desktopWallpaper} />

        <IslandPill
          currentMode={sim.currentMode}
          isHovered={sim.isHovered}
          targetWidth={sim.targetWidth}
          targetHeight={sim.targetHeight}
          borderRadius={sim.borderRadius}
          pillOpacity={sim.pillOpacity}
        />

        <div class={styles.desktopTip}>💡 Hover inside screen to trigger hover expansion!</div>
      </div>

      <ControlPanel
        currentMode={sim.currentMode}
        setCurrentMode={sim.setCurrentMode}
        sizeScale={sim.sizeScale}
        setSizeScale={sim.setSizeScale}
        pillOpacity={sim.pillOpacity}
        setPillOpacity={sim.setPillOpacity}
        activeThemeColor={sim.activeThemeColor}
        setActiveThemeColor={sim.setActiveThemeColor}
      />
    </div>
  );
}
