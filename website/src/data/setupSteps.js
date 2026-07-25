export const standaloneSteps = [
  {
    step: 1,
    title: 'Download DynamicIsland.exe',
    desc: 'Get the latest pre-compiled standalone release from GitHub. Zero installation needed.'
  },
  {
    step: 2,
    title: 'Launch Executable',
    desc: 'Double-click to start. The island will automatically align at top-center of your primary display.'
  },
  {
    step: 3,
    title: 'Configure Settings',
    desc: 'Right-click the island to customize auto-hide timers, size scale, and startup settings.'
  }
];

export const windhawkSteps = [
  {
    step: 1,
    title: 'Open Windhawk Mod Manager',
    desc: 'Click "Developer Writing Mod" in the Windhawk desktop application.'
  },
  {
    step: 2,
    title: 'Paste Source Code',
    desc: 'Copy source code from window-dynamic-island.wh.cpp into the Windhawk code editor.'
  },
  {
    step: 3,
    title: 'Compile & Inject',
    desc: 'Click "Compile and Inject" to run Dynamic Island natively inside Windows Explorer.'
  }
];

export const windhawkCodeSnippet = `// Win32 Standalone / Windhawk Injection Hook
#define WINDHAWK_META_NAME L"Window Dynamic Island"
#define WINDHAWK_META_VERSION L"1.4.1"
#define WINDHAWK_META_AUTHOR L"Aonik Yadav"

#include <windows.h>
#include <d2d1.h>
// Compiled natively with Direct2D & WinRT API integration...`;
