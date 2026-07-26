import React from 'react';
import { Header } from './components/layout/Header';
import { Footer } from './components/layout/Footer';
import { HeroSection } from './components/hero/HeroSection';
import { LiveShowcase } from './components/simulator/LiveShowcase';
import { FeatureGrid } from './components/features/FeatureGrid';
import { ScreenshotGallery } from './components/gallery/ScreenshotGallery';
import { InstallationGuide } from './components/setup/InstallationGuide';

export function App() {
  return (
    <div style={{ position: 'relative', overflowX: 'hidden' }}>
      <Header />
      
      <main>
        <HeroSection />
        <LiveShowcase />
        <FeatureGrid />
        <ScreenshotGallery />
        <InstallationGuide />
      </main>

      <Footer />
    </div>
  );
}

export default App;
