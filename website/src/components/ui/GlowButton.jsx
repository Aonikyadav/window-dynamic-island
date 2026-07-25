import React from 'react';
import styles from './GlowButton.module.css';

export function GlowButton({ children, href, variant = 'primary', icon, onClick }) {
  const className = `${styles.btn} ${variant === 'primary' ? styles.primary : styles.secondary}`;

  if (href) {
    return (
      <a href={href} class={className} target={href.startsWith('http') ? '_blank' : '_self'} rel="noreferrer">
        {icon && <i class={icon} />}
        {children}
      </a>
    );
  }

  return (
    <button class={className} onClick={onClick}>
      {icon && <i class={icon} />}
      {children}
    </button>
  );
}
