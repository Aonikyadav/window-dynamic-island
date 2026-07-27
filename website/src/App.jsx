import React, { useState } from 'react';
import { Header } from './components/layout/Header';
import { Footer } from './components/layout/Footer';
import { SideRail } from './components/effects/SideRail';
import { MouseGlow } from './components/effects/MouseGlow';
import { AuraBackground } from './components/effects/AuraBackground';
import { Particles } from './components/effects/Particles';
import { NeonGrid } from './components/effects/NeonGrid';
import { SectionDivider } from './components/effects/SectionDivider';

import { HeroSection } from './components/hero/HeroSection';
import { InteractiveSimulator } from './components/simulator/InteractiveSimulator';
import { AiAssistantSection } from './components/ai/AiAssistantSection';
import { FeatureGrid } from './components/features/FeatureGrid';
import { InstallationGuide } from './components/setup/InstallationGuide';
import { DownloadModal } from './components/download/DownloadModal';

export function App() {
  const [isDownloadOpen, setIsDownloadOpen] = useState(false);

  const handleOpenDownload = () => {
    setIsDownloadOpen(true);
  };

  const handleCloseDownload = () => {
    setIsDownloadOpen(false);
  };

  return (
    <div style={{ position: 'relative', overflowX: 'hidden' }}>
      {/* Background Atmosphere & Ambient Lighting */}
      <AuraBackground />
      <Particles />
      <MouseGlow />
      <SideRail />

      <Header onOpenDownload={handleOpenDownload} />
      
      <main style={{ position: 'relative' }}>
        <NeonGrid />
        <HeroSection onOpenDownload={handleOpenDownload} />

        <SectionDivider />
        <InteractiveSimulator onOpenDownload={handleOpenDownload} />

        <SectionDivider />
        <AiAssistantSection onOpenDownload={handleOpenDownload} />

        <SectionDivider />
        <FeatureGrid onOpenDownload={handleOpenDownload} />

        <SectionDivider />
        <InstallationGuide onOpenDownload={handleOpenDownload} />
      </main>

      <Footer onOpenDownload={handleOpenDownload} />

      {/* Registration & Download Modal */}
      <DownloadModal isOpen={isDownloadOpen} onClose={handleCloseDownload} />
    </div>
  );
}

export default App;
