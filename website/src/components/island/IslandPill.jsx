import React from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import styles from './IslandPill.module.css';

import { IdleMode } from './modes/IdleMode';
import { MediaMode } from './modes/MediaMode';
import { WeatherMode } from './modes/WeatherMode';
import { SystemMode } from './modes/SystemMode';
import { SttMode } from './modes/SttMode';
import { AssistantMode } from './modes/AssistantMode';
import { BatteryMode } from './modes/BatteryMode';
import { ClipboardMode } from './modes/ClipboardMode';

const modeComponents = {
  idle: IdleMode,
  media: MediaMode,
  weather: WeatherMode,
  system: SystemMode,
  stt: SttMode,
  assistant: AssistantMode,
  battery: BatteryMode,
  clipboard: ClipboardMode
};

export function IslandPill({
  currentMode,
  isHovered,
  targetWidth,
  targetHeight,
  borderRadius,
  pillOpacity
}) {
  const ModeComponent = modeComponents[currentMode] || IdleMode;

  return (
    <motion.div
      class={styles.pill}
      animate={{
        width: targetWidth,
        height: targetHeight,
        borderRadius: borderRadius,
        opacity: pillOpacity
      }}
      transition={{
        type: 'spring',
        stiffness: 220,
        damping: 24
      }}
    >
      <AnimatePresence mode="wait">
        <motion.div
          key={`${currentMode}-${isHovered}`}
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
          transition={{ duration: 0.18 }}
          style={{ width: '100%', height: '100%' }}
        >
          <ModeComponent isExpanded={isHovered} />
        </motion.div>
      </AnimatePresence>
    </motion.div>
  );
}
