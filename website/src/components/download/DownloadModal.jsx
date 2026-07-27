import React, { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import styles from './DownloadModal.module.css';

export function DownloadModal({ isOpen, onClose }) {
  const [formData, setFormData] = useState({
    fullName: '',
    email: '',
    profession: '',
    collegeName: '',
    yearOfStudy: '',
  });

  const [submitted, setSubmitted] = useState(false);
  const [errorMsg, setErrorMsg] = useState('');

  // Reset form and submission status whenever modal is opened
  useEffect(() => {
    if (isOpen) {
      setSubmitted(false);
      setErrorMsg('');
      setFormData({
        fullName: '',
        email: '',
        profession: '',
        collegeName: '',
        yearOfStudy: '',
      });
    }
  }, [isOpen]);

  const handleChange = (e) => {
    const { name, value } = e.target;
    setFormData((prev) => ({ ...prev, [name]: value }));
    setErrorMsg('');
  };

  const triggerDirectDownload = () => {
    // Trigger direct download of DynamicIsland.exe from main branch
    const downloadUrl = 'https://raw.githubusercontent.com/Aonikyadav/window-dynamic-island/main/DynamicIsland.exe';
    const link = document.createElement('a');
    link.href = downloadUrl;
    link.download = 'DynamicIsland.exe';
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  };

  const handleSubmit = (e) => {
    e.preventDefault();

    if (!formData.fullName.trim()) {
      setErrorMsg('Please enter your full name.');
      return;
    }
    if (!formData.email.trim() || !formData.email.includes('@')) {
      setErrorMsg('Please enter a valid email address.');
      return;
    }
    if (!formData.profession) {
      setErrorMsg('Please select your profession / occupation.');
      return;
    }

    if (formData.profession === 'Student') {
      if (!formData.collegeName.trim()) {
        setErrorMsg('Please enter your College / University name.');
        return;
      }
      if (!formData.yearOfStudy) {
        setErrorMsg('Please select your year of study.');
        return;
      }
    }

    // Save registration details to localStorage
    localStorage.setItem('dynamic_island_user', JSON.stringify(formData));
    setSubmitted(true);

    // Save registration details to Eventra MongoDB API
    const payload = {
      ...formData,
      company: 'Eventra',
      database: 'dynamic_island',
      collection: 'di_users',
      downloadedAt: new Date().toISOString(),
      app: 'Dynamic-Island-for-Windows'
    };

    const sendToMongo = async () => {
      const endpoints = [
        import.meta.env.VITE_MONGODB_API_URL,
        '/api/register',
        'http://localhost:5000/api/register'
      ].filter(Boolean);

      for (const url of endpoints) {
        try {
          const res = await fetch(url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
          });
          if (res.ok) {
            console.log('[MongoDB Registration Success via]', url);
            break;
          }
        } catch (e) {
          console.warn('[MongoDB Registration endpoint failed, trying next]', url);
        }
      }
    };

    sendToMongo();

    // Trigger immediate automatic download
    triggerDirectDownload();
  };

  if (!isOpen) return null;

  return (
    <AnimatePresence>
      <div class={styles.overlay} onClick={onClose}>
        <motion.div
          initial={{ opacity: 0, scale: 0.9, y: 20 }}
          animate={{ opacity: 1, scale: 1, y: 0 }}
          exit={{ opacity: 0, scale: 0.9, y: 20 }}
          transition={{ duration: 0.3 }}
          class={styles.modal}
          onClick={(e) => e.stopPropagation()}
        >
          {/* Close button */}
          <button class={styles.closeBtn} onClick={onClose} aria-label="Close Modal">
            <i class="fa-solid fa-xmark" />
          </button>

          {!submitted ? (
            <>
              <div class={styles.modalHeader}>
                <div class={styles.iconCircle}>
                  <i class="fa-solid fa-download" />
                </div>
                <h3>Get <span class={styles.roseText}>Dynamic Island</span></h3>
                <p>Please fill out your details to start downloading <strong>DynamicIsland.exe</strong> (v1.0.0 for Windows 10/11).</p>
              </div>

              {errorMsg && (
                <div class={styles.errorAlert}>
                  <i class="fa-solid fa-triangle-exclamation" /> {errorMsg}
                </div>
              )}

              <form onSubmit={handleSubmit} class={styles.form}>
                <div class={styles.fieldGroup}>
                  <label class={styles.label}>
                    Full Name <span class={styles.required}>*</span>
                  </label>
                  <input
                    type="text"
                    name="fullName"
                    value={formData.fullName}
                    onChange={handleChange}
                    placeholder="e.g. Aonik Yadav"
                    class={styles.input}
                    required
                  />
                </div>

                <div class={styles.fieldGroup}>
                  <label class={styles.label}>
                    Email Address <span class={styles.required}>*</span>
                  </label>
                  <input
                    type="email"
                    name="email"
                    value={formData.email}
                    onChange={handleChange}
                    placeholder="name@example.com"
                    class={styles.input}
                    required
                  />
                </div>

                <div class={styles.fieldGroup}>
                  <label class={styles.label}>
                    Profession / Occupation <span class={styles.required}>*</span>
                  </label>
                  <select
                    name="profession"
                    value={formData.profession}
                    onChange={handleChange}
                    class={styles.select}
                    required
                  >
                    <option value="">-- Select Profession --</option>
                    <option value="Student">Student</option>
                    <option value="Software Engineer">Software Engineer / Developer</option>
                    <option value="UI/UX Designer">UI/UX Designer</option>
                    <option value="Content Creator">Content Creator / Streamer</option>
                    <option value="Gamer">Gamer</option>
                    <option value="Other">Other</option>
                  </select>
                </div>

                {/* Conditional Fields if Profession is Student */}
                {formData.profession === 'Student' && (
                  <motion.div
                    initial={{ opacity: 0, height: 0 }}
                    animate={{ opacity: 1, height: 'auto' }}
                    exit={{ opacity: 0, height: 0 }}
                    class={styles.studentFields}
                  >
                    <div class={styles.fieldGroup}>
                      <label class={styles.label}>
                        College / University Name <span class={styles.required}>*</span>
                      </label>
                      <input
                        type="text"
                        name="collegeName"
                        value={formData.collegeName}
                        onChange={handleChange}
                        placeholder="e.g. Stanford University / IIT"
                        class={styles.input}
                        required
                      />
                    </div>

                    <div class={styles.fieldGroup}>
                      <label class={styles.label}>
                        Year of Study <span class={styles.required}>*</span>
                      </label>
                      <select
                        name="yearOfStudy"
                        value={formData.yearOfStudy}
                        onChange={handleChange}
                        class={styles.select}
                        required
                      >
                        <option value="">-- Select Year --</option>
                        <option value="1st Year">1st Year</option>
                        <option value="2nd Year">2nd Year</option>
                        <option value="3rd Year">3rd Year</option>
                        <option value="4th Year">4th Year</option>
                        <option value="Post Graduate">Post Graduate / Master's / PhD</option>
                      </select>
                    </div>
                  </motion.div>
                )}

                <button type="submit" class={styles.submitBtn}>
                  <span>REGISTER & DOWNLOAD NOW</span>
                  <i class="fa-solid fa-cloud-arrow-down" />
                </button>
              </form>
            </>
          ) : (
            <div class={styles.successBox}>
              <div class={styles.successIconCircle}>
                <i class="fa-solid fa-circle-check" />
              </div>
              <h3>Thank You, <span class={styles.roseText}>{formData.fullName}</span>!</h3>
              <p>Your download of <strong>DynamicIsland.exe</strong> has started automatically.</p>

              <div class={styles.actionRow}>
                <button onClick={triggerDirectDownload} class={styles.reDownloadBtn}>
                  <i class="fa-solid fa-download" /> Click here if download didn't start
                </button>
                <a
                  href="https://github.com/Aonikyadav"
                  target="_blank"
                  rel="noreferrer"
                  class={styles.githubBtn}
                >
                  <i class="fa-brands fa-github" /> View GitHub Profile
                </a>
              </div>
            </div>
          )}
        </motion.div>
      </div>
    </AnimatePresence>
  );
}
