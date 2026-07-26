import React, { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import styles from './InteractiveSimulator.module.css';

export function InteractiveSimulator() {
  const [activeState, setActiveState] = useState('idle');
  const [scaleFactor, setScaleFactor] = useState(1.0);
  const [pillOpacity, setPillOpacity] = useState(1.0);
  const [isHovered, setIsHovered] = useState(false);
  const [isAutoPlay, setIsAutoPlay] = useState(false);

  const states = [
    { id: 'idle', label: 'Idle Clock', icon: 'fa-clock' },
    { id: 'airpods', label: 'AirPods', icon: 'fa-headphones' },
    { id: 'media', label: 'Media Player', icon: 'fa-music' },
    { id: 'weather', label: 'Weather', icon: 'fa-cloud-sun' },
    { id: 'calendar', label: 'Calendar', icon: 'fa-calendar-days' },
    { id: 'battery', label: 'Battery', icon: 'fa-battery-three-quarters' },
    { id: 'stt', label: 'Speech-to-Text', icon: 'fa-microphone' },
    { id: 'system', label: 'System Stats', icon: 'fa-chart-pie' },
    { id: 'autohide', label: 'Auto-Hide', icon: 'fa-eye-slash' }
  ];

  // Auto-play sequence mode
  useEffect(() => {
    if (!isAutoPlay) return;

    const sequence = ['idle', 'airpods', 'media', 'weather', 'calendar', 'battery', 'stt', 'system', 'autohide'];
    let currentIdx = 0;

    const interval = setInterval(() => {
      currentIdx = (currentIdx + 1) % sequence.length;
      setActiveState(sequence[currentIdx]);
    }, 3500);

    return () => clearInterval(interval);
  }, [isAutoPlay]);

  // Determine width & height of the pill depending on state
  const getPillDimensions = () => {
    if (activeState === 'autohide' && !isHovered) {
      return { width: 0, height: 0, opacity: 0, borderRadius: 99 };
    }
    if (isHovered && activeState === 'idle') {
      return { width: 340, height: 48, opacity: pillOpacity, borderRadius: 24 };
    }

    switch (activeState) {
      case 'idle':
        return { width: 190, height: 34, opacity: pillOpacity, borderRadius: 99 };
      case 'airpods':
        return { width: 260, height: 46, opacity: pillOpacity, borderRadius: 24 };
      case 'media':
        return { width: 360, height: 75, opacity: pillOpacity, borderRadius: 24 };
      case 'weather':
        return { width: 320, height: 70, opacity: pillOpacity, borderRadius: 22 };
      case 'calendar':
        return { width: 300, height: 68, opacity: pillOpacity, borderRadius: 22 };
      case 'battery':
        return { width: 250, height: 44, opacity: pillOpacity, borderRadius: 22 };
      case 'stt':
        return { width: 340, height: 60, opacity: pillOpacity, borderRadius: 24 };
      case 'system':
        return { width: 330, height: 68, opacity: pillOpacity, borderRadius: 22 };
      case 'autohide':
        return { width: 220, height: 38, opacity: pillOpacity, borderRadius: 99 };
      default:
        return { width: 190, height: 34, opacity: pillOpacity, borderRadius: 99 };
    }
  };

  const dim = getPillDimensions();

  return (
    <section id="simulator" class={styles.section}>
      <div class={styles.container}>
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true }}
          transition={{ duration: 0.6 }}
          class={styles.header}
        >
          <h2>
            Experience <span class={styles.roseGradient}>Dynamic Island Live</span>
          </h2>
          <p>Test the actual real-time application behavior, expanded widgets, hover transitions, and settings sliders right inside your browser.</p>
        </motion.div>

        {/* State Pills Controls Bar */}
        <div class={styles.pillsBar}>
          <button
            class={`${styles.autoDemoBtn} ${isAutoPlay ? styles.pillBtnActive : ''}`}
            onClick={() => setIsAutoPlay(!isAutoPlay)}
          >
            <i class={`fa-solid ${isAutoPlay ? 'fa-pause' : 'fa-play'}`} />
            <span>{isAutoPlay ? 'Pause Auto Demo' : 'Run Auto Demo'}</span>
          </button>

          {states.map((s) => (
            <button
              key={s.id}
              class={`${styles.pillBtn} ${activeState === s.id && !isAutoPlay ? styles.pillBtnActive : ''}`}
              onClick={() => {
                setIsAutoPlay(false);
                setActiveState(s.id);
              }}
            >
              <i class={`fa-solid ${s.icon}`} />
              <span>{s.label}</span>
            </button>
          ))}
        </div>

        {/* Desktop Viewport & Settings Dashboard Grid */}
        <div class={styles.desktopGrid}>
          {/* Simulated Windows Desktop */}
          <div class={styles.desktopViewport}>
            <div class={styles.desktopWallpaper} />
            <span class={styles.desktopTip}>
              <i class="fa-solid fa-hand-pointer" /> Hover over the island to expand
            </span>

            {/* Live Animated Morphing Island Pill */}
            <motion.div
              animate={{
                width: dim.width * scaleFactor,
                height: dim.height * scaleFactor,
                opacity: dim.opacity,
                borderRadius: dim.borderRadius
              }}
              transition={{ type: 'spring', stiffness: 240, damping: 22 }}
              onMouseEnter={() => setIsHovered(true)}
              onMouseLeave={() => setIsHovered(false)}
              class={styles.simPill}
            >
              <AnimatePresence mode="wait">
                {activeState === 'idle' && (
                  <motion.div
                    key="idle"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', gap: '0.8rem', padding: '0 0.8rem', fontSize: '0.8rem', fontWeight: 600 }}
                  >
                    <span>18:28</span>
                    <span style={{ color: 'var(--rose-primary)' }}>●</span>
                    <span style={{ color: 'var(--text-muted)' }}>Windows 11</span>
                  </motion.div>
                )}

                {activeState === 'airpods' && (
                  <motion.div
                    key="airpods"
                    initial={{ opacity: 0, scale: 0.9 }}
                    animate={{ opacity: 1, scale: 1 }}
                    exit={{ opacity: 0, scale: 0.9 }}
                    style={{ display: 'flex', alignItems: 'center', gap: '0.8rem', padding: '0 1rem', fontSize: '0.85rem' }}
                  >
                    <i class="fa-solid fa-headphones" style={{ color: 'var(--rose-primary)', fontSize: '1.1rem' }} />
                    <div style={{ display: 'flex', flexDirection: 'column' }}>
                      <span style={{ fontWeight: 700 }}>AirPods Pro</span>
                      <span style={{ fontSize: '0.7rem', color: '#4ade80' }}>Connected • 95%</span>
                    </div>
                  </motion.div>
                )}

                {activeState === 'media' && (
                  <motion.div
                    key="media"
                    initial={{ opacity: 0, y: 5 }}
                    animate={{ opacity: 1, y: 0 }}
                    exit={{ opacity: 0, y: -5 }}
                    style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', padding: '0 1rem' }}
                  >
                    <div style={{ display: 'flex', alignItems: 'center', gap: '0.7rem' }}>
                      <div style={{ width: 34, height: 34, borderRadius: '50%', background: 'linear-gradient(45deg, var(--rose-primary), #000)', display: 'flex', alignItems: 'center', justifyCenter: 'center' }}>
                        <i class="fa-solid fa-compact-disc" style={{ animation: 'rotate-spinner 6s linear infinite', color: '#fff', fontSize: '1rem' }} />
                      </div>
                      <div style={{ display: 'flex', flexDirection: 'column' }}>
                        <span style={{ fontWeight: 700, fontSize: '0.8rem' }}>Super Saiyan Rosé</span>
                        <span style={{ fontSize: '0.7rem', color: 'var(--text-muted)' }}>Goku Black • Theme</span>
                      </div>
                    </div>
                    <div style={{ display: 'flex', alignItems: 'center', gap: '0.6rem', color: 'var(--rose-primary)' }}>
                      <i class="fa-solid fa-backward-step" style={{ cursor: 'pointer' }} />
                      <i class="fa-solid fa-pause" style={{ cursor: 'pointer', fontSize: '1rem' }} />
                      <i class="fa-solid fa-forward-step" style={{ cursor: 'pointer' }} />
                    </div>
                  </motion.div>
                )}

                {activeState === 'weather' && (
                  <motion.div
                    key="weather"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', padding: '0 1.2rem' }}
                  >
                    <div style={{ display: 'flex', alignItems: 'center', gap: '0.8rem' }}>
                      <i class="fa-solid fa-cloud-bolt" style={{ color: 'var(--rose-primary)', fontSize: '1.5rem' }} />
                      <div style={{ display: 'flex', flexDirection: 'column' }}>
                        <span style={{ fontWeight: 800, fontSize: '1.1rem' }}>24°C</span>
                        <span style={{ fontSize: '0.7rem', color: 'var(--text-muted)' }}>Thunderstorm • Tokyo</span>
                      </div>
                    </div>
                    <span style={{ fontSize: '0.75rem', color: 'var(--rose-primary)', fontWeight: 600 }}>H:28° L:19°</span>
                  </motion.div>
                )}

                {activeState === 'calendar' && (
                  <motion.div
                    key="calendar"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', padding: '0 1.2rem' }}
                  >
                    <div style={{ display: 'flex', alignItems: 'center', gap: '0.8rem' }}>
                      <i class="fa-solid fa-calendar-check" style={{ color: 'var(--rose-primary)', fontSize: '1.3rem' }} />
                      <div style={{ display: 'flex', flexDirection: 'column' }}>
                        <span style={{ fontWeight: 700, fontSize: '0.85rem' }}>Team Sync Meeting</span>
                        <span style={{ fontSize: '0.7rem', color: 'var(--text-muted)' }}>19:00 - 20:00 • In 32 mins</span>
                      </div>
                    </div>
                  </motion.div>
                )}

                {activeState === 'battery' && (
                  <motion.div
                    key="battery"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', gap: '0.9rem', padding: '0 1.2rem', fontSize: '0.85rem' }}
                  >
                    <i class="fa-solid fa-bolt-lightning" style={{ color: '#4ade80', fontSize: '1.2rem' }} />
                    <span style={{ fontWeight: 800 }}>88% Charged</span>
                    <span style={{ fontSize: '0.7rem', color: 'var(--text-muted)' }}>Power Adapter Connected</span>
                  </motion.div>
                )}

                {activeState === 'stt' && (
                  <motion.div
                    key="stt"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', width: '100%', padding: '0 1rem' }}
                  >
                    <div style={{ display: 'flex', alignItems: 'center', gap: '0.6rem' }}>
                      <i class="fa-solid fa-microphone-lines" style={{ color: 'var(--rose-primary)', animation: 'live-flash 1s infinite' }} />
                      <span style={{ fontSize: '0.8rem', fontWeight: 600 }}>Listening dictation...</span>
                    </div>
                    <span style={{ background: 'var(--rose-subtle)', border: '1px solid var(--rose-primary)', color: 'var(--rose-primary)', fontSize: '0.65rem', padding: '0.2rem 0.6rem', borderRadius: 99, fontWeight: 700 }}>LIVE</span>
                  </motion.div>
                )}

                {activeState === 'system' && (
                  <motion.div
                    key="system"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    exit={{ opacity: 0 }}
                    style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-around', width: '100%', padding: '0 1rem', fontSize: '0.75rem' }}
                  >
                    <div><span style={{ color: 'var(--text-muted)' }}>CPU:</span> <strong style={{ color: 'var(--rose-primary)' }}>38%</strong></div>
                    <div><span style={{ color: 'var(--text-muted)' }}>RAM:</span> <strong style={{ color: 'var(--rose-primary)' }}>54%</strong></div>
                    <div><span style={{ color: 'var(--text-muted)' }}>FPS:</span> <strong style={{ color: '#4ade80' }}>60</strong></div>
                  </motion.div>
                )}

                {activeState === 'autohide' && isHovered && (
                  <motion.div
                    key="autohide-hover"
                    initial={{ opacity: 0 }}
                    animate={{ opacity: 1 }}
                    style={{ fontSize: '0.75rem', fontWeight: 600, color: 'var(--rose-primary)' }}
                  >
                    Auto-Unhidden on Hover!
                  </motion.div>
                )}
              </AnimatePresence>
            </motion.div>
          </div>

          {/* Interactive Customization Dashboard */}
          <div class={styles.controlsBox}>
            <h3 class={styles.controlsTitle}>Live Parameter Customizer</h3>
            <p class={styles.controlsDesc}>
              Adjust scale factors and opacity parameters to see how Dynamic Island seamlessly adapts to your custom desktop configuration.
            </p>

            <div class={styles.sliderRow}>
              <span>Island Size Scale</span>
              <input
                type="range"
                min="0.8"
                max="1.3"
                step="0.05"
                value={scaleFactor}
                onChange={(e) => setScaleFactor(parseFloat(e.target.value))}
              />
              <span class={styles.valTag}>{Math.round(scaleFactor * 100)}%</span>
            </div>

            <div class={styles.sliderRow}>
              <span>Pill Opacity</span>
              <input
                type="range"
                min="0.4"
                max="1.0"
                step="0.05"
                value={pillOpacity}
                onChange={(e) => setPillOpacity(parseFloat(e.target.value))}
              />
              <span class={styles.valTag}>{Math.round(pillOpacity * 100)}%</span>
            </div>

            <div style={{ background: 'rgba(255, 42, 133, 0.06)', border: '1px solid var(--border-active)', padding: '1rem', borderRadius: 12, fontSize: '0.85rem', color: 'var(--text-muted)', display: 'flex', gap: '0.8rem', alignItems: 'center' }}>
              <i class="fa-solid fa-circle-info" style={{ color: 'var(--rose-primary)', fontSize: '1.2rem' }} />
              <span>
                <strong>Zero Latency Physics:</strong> The island utilizes dual-spring velocity interpolation (`stiffness 240, damping 22`) for organic fluid resizing.
              </span>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
}
