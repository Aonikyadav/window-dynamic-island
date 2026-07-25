import React, { useState } from 'react';
import styles from './CodeBox.module.css';

export function CodeBox({ code }) {
  const [copied, setCopied] = useState(false);

  const handleCopy = () => {
    navigator.clipboard.writeText(code);
    setCopied(true);
    setTimeout(() => setCopied(false), 2000);
  };

  return (
    <div class={styles.box}>
      <pre><code>{code}</code></pre>
      <button class={styles.copyBtn} onClick={handleCopy}>
        <i class={copied ? 'fa-solid fa-check' : 'fa-regular fa-copy'} />
        {copied ? ' Copied!' : ' Copy'}
      </button>
    </div>
  );
}
