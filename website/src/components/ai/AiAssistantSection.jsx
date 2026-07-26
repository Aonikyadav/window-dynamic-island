import React, { useState, useEffect } from 'react';
import { motion } from 'framer-motion';
import styles from './AiAssistantSection.module.css';

export function AiAssistantSection() {
  const samplePhrases = [
    "Hey Jarvis, set a timer for 15 minutes and play Goku Black Rosé theme.",
    "Speech-to-Text: Dictating live notes directly to active window...",
    "System status check: CPU temperature 42°C, 60 FPS lock engaged.",
    "Copying text from clipboard... Synchronized with Dynamic Island."
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
  }, [charIdx, phraseIdx]);

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
            Jarvis Voice AI & <span class={styles.roseGradient}>Live Speech-to-Text</span>
          </h2>
          <p>Dictate seamlessly anywhere in Windows or issue voice commands to control your desktop with zero latency.</p>
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

            <div class={styles.sttConsole}>
              <div class={styles.consoleHeader}>
                <span><i class="fa-solid fa-microphone" style={{ color: 'var(--rose-primary)' }} /> SPEECH ENGINE DICTATION</span>
                <span style={{ color: '#4ade80' }}>ACTIVE</span>
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
                <i class="fa-solid fa-wand-magic-sparkles" /> System-Wide Dictation
              </h3>
              <p class={styles.cardDesc}>
                Speak naturally to type into any application—Notepad, Word, Discord, or your IDE—with instant WinRT speech recognition.
              </p>
            </div>

            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-robot" /> Voice Assistant Integration
              </h3>
              <p class={styles.cardDesc}>
                Trigger actions hands-free. Command media playback, query system diagnostics, or search without leaving your active workflow.
              </p>
            </div>

            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-shield-halved" /> 100% On-Device Privacy
              </h3>
              <p class={styles.cardDesc}>
                All speech processing runs locally using Windows native Speech APIs. Zero cloud data leaks, zero background tracking.
              </p>
            </div>
          </motion.div>
        </div>
      </div>
    </section>
  );
}
