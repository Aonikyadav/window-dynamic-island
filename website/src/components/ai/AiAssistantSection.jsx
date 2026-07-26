import React, { useState, useEffect } from 'react';
import { motion } from 'framer-motion';
import styles from './AiAssistantSection.module.css';

export function AiAssistantSection() {
  const [customWakeWord, setCustomWakeWord] = useState('Hey Nova');

  const presetPhrases = [
    'Hey Jarvis',
    'Hey Nova',
    'Computer',
    'Friday',
    'Athena',
    'Hey Baby'
  ];

  const samplePhrases = [
    `"${customWakeWord}, set a timer for 15 minutes and play Goku Black Rosé theme."`,
    `"${customWakeWord}, dictating live notes directly to my active document..."`,
    `"${customWakeWord}, check system status and performance metrics."`,
    `"${customWakeWord}, copy text from clipboard and sync to Dynamic Island."`
  ];

  const [phraseIdx, setPhraseIdx] = useState(0);
  const [charIdx, setCharIdx] = useState(0);

  useEffect(() => {
    const currentPhrase = samplePhrases[phraseIdx];
    if (charIdx < currentPhrase.length) {
      const timer = setTimeout(() => setCharIdx(charIdx + 1), 35);
      return () => clearTimeout(timer);
    } else {
      const resetTimer = setTimeout(() => {
        setCharIdx(0);
        setPhraseIdx((prev) => (prev + 1) % samplePhrases.length);
      }, 3000);
      return () => clearTimeout(resetTimer);
    }
  }, [charIdx, phraseIdx, customWakeWord]);

  return (
    <section id="ai" class={styles.section}>
      <div class={styles.container}>
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true }}
          transition={{ duration: 0.6 }}
          class={styles.header}
        >
          <h2>
            Customizable Wake Word & <span class={styles.roseGradient}>Voice AI Assistant</span>
          </h2>
          <p>Personalize your activation phrase directly from Settings. Say any phrase or press Ctrl + Space to trigger your desktop assistant hands-free.</p>
        </motion.div>

        <div class={styles.grid}>
          {/* Animated AI Assistant Box */}
          <motion.div
            initial={{ opacity: 0, x: -30 }}
            whileInView={{ opacity: 1, x: 0 }}
            viewport={{ once: true }}
            transition={{ duration: 0.7 }}
            class={styles.aiBox}
          >
            <div class={styles.orbWrapper}>
              <div class={styles.orbRing} />
              <div class={styles.orbCore} />
            </div>

            <div class={styles.voiceWave}>
              <span /><span /><span /><span /><span /><span />
            </div>

            {/* Interactive Wake Word Configuration Widget */}
            <div class={styles.wakeWordConfigBox}>
              <span class={styles.configLabel}>
                <i class="fa-solid fa-gear" style={{ color: 'var(--rose-primary)', marginRight: 6 }} />
                Custom Wake Word Setting
              </span>
              <div class={styles.inputGroup}>
                <input
                  type="text"
                  value={customWakeWord}
                  onChange={(e) => {
                    setCustomWakeWord(e.target.value);
                    setCharIdx(0);
                  }}
                  placeholder="Enter custom wake phrase..."
                  class={styles.wakeInput}
                />
              </div>
              <div class={styles.presetsRow}>
                {presetPhrases.map((phrase) => (
                  <button
                    key={phrase}
                    class={`${styles.presetChip} ${customWakeWord === phrase ? styles.presetChipActive : ''}`}
                    onClick={() => {
                      setCustomWakeWord(phrase);
                      setCharIdx(0);
                    }}
                  >
                    {phrase}
                  </button>
                ))}
              </div>
            </div>

            {/* Live Dictation / Voice Matcher Console */}
            <div class={styles.sttConsole}>
              <div class={styles.consoleHeader}>
                <span><i class="fa-solid fa-microphone" style={{ color: 'var(--rose-primary)' }} /> WAKE WORD LISTENER</span>
                <span style={{ color: '#4ade80' }}>DYNAMIC MATCH</span>
              </div>
              <div class={styles.typeText}>
                {samplePhrases[phraseIdx].substring(0, charIdx)}
                <span class={styles.cursor} />
              </div>
            </div>
          </motion.div>

          {/* Feature Highlight Cards */}
          <motion.div
            initial={{ opacity: 0, x: 30 }}
            whileInView={{ opacity: 1, x: 0 }}
            viewport={{ once: true }}
            transition={{ duration: 0.7, delay: 0.2 }}
            class={styles.rightInfo}
          >
            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-sliders" /> Fully Customizable Activation Phrase
              </h3>
              <p class={styles.cardDesc}>
                Type any custom phrase in Settings (*Hey Jarvis*, *Hey Nova*, *Computer*, *Friday*, *Athena*). The C++ WinRT speech engine instantly updates recognition list constraints.
              </p>
            </div>

            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-keyboard" /> Push-To-Talk (Ctrl + Space)
              </h3>
              <p class={styles.cardDesc}>
                Prefer hotkeys over voice? **Ctrl + Space** remains instantly available as a secondary push-to-talk activation shortcut anytime.
              </p>
            </div>

            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-floppy-disk" /> Persisted Configuration
              </h3>
              <p class={styles.cardDesc}>
                Your custom wake phrase is saved directly to application settings and persists automatically across computer reboots.
              </p>
            </div>
          </motion.div>
        </div>
      </div>
    </section>
  );
}
