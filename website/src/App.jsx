import React from 'react';
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

export function App() {
  return (
    <div style={{ position: 'relative', overflowX: 'hidden' }}>
      {/* Background Atmosphere & Ambient Lighting */}
      <AuraBackground />
      <Particles />
      <MouseGlow />
      <SideRail />

      <Header />
      
      <main style={{ position: 'relative' }}>
        <NeonGrid />
        <HeroSection />

        <SectionDivider />
        <InteractiveSimulator />

        <SectionDivider />
        <AiAssistantSection />

        <SectionDivider />
        <FeatureGrid />

        <SectionDivider />
        <InstallationGuide />
      </main>

      <Footer />
    </div>
  );
}

export default App;
