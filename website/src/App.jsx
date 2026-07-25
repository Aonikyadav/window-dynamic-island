import React from 'react';
import { Header } from './components/layout/Header';
import { Footer } from './components/layout/Footer';
import { AuraBackground } from './components/effects/AuraBackground';
import { Particles } from './components/effects/Particles';
import { NeonGrid } from './components/effects/NeonGrid';

import { Hero } from './components/sections/Hero';
import { Stats } from './components/sections/Stats';
import { Simulator } from './components/sections/Simulator';
import { Features } from './components/sections/Features';
import { Screenshots } from './components/sections/Screenshots';
import { SetupGuide } from './components/sections/SetupGuide';

export function App() {
  return (
    <div style={{ position: 'relative', overflowX: 'hidden' }}>
      <AuraBackground />
      <Particles />
      
      <Header />
      
      <main>
        <div style={{ position: 'relative' }}>
          <NeonGrid />
          <Hero />
          <Stats />
        </div>
        
        <Simulator />
        <Features />
        <Screenshots />
        <SetupGuide />
      </main>

      <Footer />
    </div>
  );
}

export default App;
