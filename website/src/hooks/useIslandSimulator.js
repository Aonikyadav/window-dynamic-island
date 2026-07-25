import { useState } from 'react';
import { modesConfig } from '../data/modes';

export function useIslandSimulator() {
  const [currentMode, setCurrentMode] = useState('idle');
  const [isHovered, setIsHovered] = useState(false);
  const [sizeScale, setSizeScale] = useState(1.0);
  const [pillOpacity, setPillOpacity] = useState(0.95);
  const [activeThemeColor, setActiveThemeColor] = useState('#ff2a85');

  const modeData = modesConfig[currentMode] || modesConfig.idle;
  const stateKey = isHovered ? 'expanded' : 'collapsed';

  const targetWidth = modeData.width[stateKey] * sizeScale;
  const targetHeight = modeData.height[stateKey] * sizeScale;
  const borderRadius = isHovered ? 24 : 99;

  return {
    currentMode,
    setCurrentMode,
    isHovered,
    setIsHovered,
    sizeScale,
    setSizeScale,
    pillOpacity,
    setPillOpacity,
    activeThemeColor,
    setActiveThemeColor,
    targetWidth,
    targetHeight,
    borderRadius,
    stateKey
  };
}
