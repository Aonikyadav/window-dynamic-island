import React, { useEffect, useState } from 'react';
import styles from './MouseGlow.module.css';

export function MouseGlow() {
  const [pos, setPos] = useState({ x: -500, y: -500 });

  useEffect(() => {
    const handleMouseMove = (e) => {
      setPos({ x: e.clientX, y: e.clientY });
    };

    window.addEventListener('mousemove', handleMouseMove);
    return () => window.removeEventListener('mousemove', handleMouseMove);
  }, []);

  return (
    <div
      class={styles.glow}
      style={{
        left: `${pos.x}px`,
        top: `${pos.y}px`
      }}
    />
  );
}
