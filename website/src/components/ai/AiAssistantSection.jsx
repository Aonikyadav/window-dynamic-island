import React, { useState, useEffect } from 'react';
import { motion } from 'framer-motion';
import styles from './AiAssistantSection.module.css';

export function AiAssistantSection() {
  const [customWakeWord, setCustomWakeWord] = useState('Hey Nova');
  const [enableAssistant, setEnableAssistant] = useState(true);
  const [enableWakeWord, setEnableWakeWord] = useState(true);
  const [sensitivity, setSensitivity] = useState('Medium');
  const [autoStart, setAutoStart] = useState(true);
  const [testingTrigger, setTestingTrigger] = useState(false);

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
    if (!enableAssistant || !enableWakeWord) return;
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
  }, [charIdx, phraseIdx, customWakeWord, enableAssistant, enableWakeWord]);

  const handleTestWakeWord = () => {
    setTestingTrigger(true);
    setCharIdx(0);
    setTimeout(() => {
      setTestingTrigger(false);
    }, 3500);
  };

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
          {/* Animated AI Assistant Settings Box */}
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

            {/* Interactive Voice Assistant Settings Widget */}
            <div class={styles.settingsWidget}>
              <div class={styles.widgetTitle}>
                <i class="fa-solid fa-sliders" /> Voice Assistant Settings
              </div>

              <label class={styles.checkboxRow}>
                <input
                  type="checkbox"
                  checked={enableAssistant}
                  onChange={(e) => setEnableAssistant(e.target.checked)}
                />
                Enable Voice Assistant
              </label>

              <label class={styles.checkboxRow}>
                <input
                  type="checkbox"
                  checked={enableWakeWord}
                  onChange={(e) => setEnableWakeWord(e.target.checked)}
                />
                Enable Wake Word Detection
              </label>

              <div class={styles.fieldGroup}>
                <span class={styles.fieldLabel}>Wake Word Phrase:</span>
                <div class={styles.wakeInputRow}>
                  <input
                    type="text"
                    value={customWakeWord}
                    onChange={(e) => {
                      setCustomWakeWord(e.target.value);
                      setCharIdx(0);
                    }}
                    placeholder="Type custom wake phrase..."
                    class={styles.wakeInput}
                    disabled={!enableWakeWord}
                  />
                  <button
                    class={styles.testBtn}
                    onClick={handleTestWakeWord}
                    disabled={!enableWakeWord}
                  >
                    <i class="fa-solid fa-play" /> {testingTrigger ? 'Listening...' : 'Test'}
                  </button>
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
                      disabled={!enableWakeWord}
                    >
                      {phrase}
                    </button>
                  ))}
                </div>
              </div>

              <div class={styles.fieldGroup}>
                <span class={styles.fieldLabel}>Recognition Sensitivity:</span>
                <select
                  value={sensitivity}
                  onChange={(e) => setSensitivity(e.target.value)}
                  class={styles.selectInput}
                  disabled={!enableWakeWord}
                >
                  <option value="Low">Low (High Confidence)</option>
                  <option value="Medium">Medium (Default)</option>
                  <option value="High">High (Lenient)</option>
                </select>
              </div>

              <label class={styles.checkboxRow}>
                <input
                  type="checkbox"
                  checked={autoStart}
                  onChange={(e) => setAutoStart(e.target.checked)}
                  disabled={!enableWakeWord}
                />
                Auto Start Listening on Boot
              </label>
            </div>

            {/* Live Dictation / Voice Matcher Console */}
            <div class={styles.sttConsole}>
              <div class={styles.consoleHeader}>
                <span><i class="fa-solid fa-microphone" style={{ color: 'var(--rose-primary)' }} /> WAKE WORD LISTENER</span>
                <span style={{ color: testingTrigger ? 'var(--rose-primary)' : '#4ade80' }}>
                  {testingTrigger ? '⚡ TEST ACTIVATED' : 'DYNAMIC MATCH'}
                </span>
              </div>
              <div class={styles.typeText}>
                {enableWakeWord ? samplePhrases[phraseIdx].substring(0, charIdx) : '[ Wake Word Disabled in Settings ]'}
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
                <i class="fa-solid fa-sliders" /> Fully Configurable Wake Phrase
              </h3>
              <p class={styles.cardDesc}>
                Type any preferred phrase directly in Settings (*Hey Jarvis*, *Hey Nova*, *Computer*, *Friday*, *Athena*, *Hey Baby*). The C++ WinRT speech engine dynamically updates recognition constraints.
              </p>
            </div>

            <div class={styles.infoCard}>
              <h3 class={styles.cardTitle}>
                <i class="fa-solid fa-comments" /> Natural Language Speech Commands
              </h3>
              <p class={styles.cardDesc}>
                Supports flexible trigger verbs (*Open*, *Launch*, *Start*, *Run*, *Take me to*, *Close*, *Exit*, *Quit*, *Kill*) across apps, websites, media, volume, battery, system status, and context pronoun resolving (*"Close it"*).
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
          </motion.div>
        </div>
      </div>
    </section>
  );
}
