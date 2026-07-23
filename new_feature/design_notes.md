# AI Voice Assistant Design Notes - Local-First Router Architecture

This document describes the modular, "Local-First" architecture for integrating a voice and click-activated AI Assistant with **Dynamic-Island-for-Windows**.

---

## 1. Core Principles
1. **Sleep-by-Default (Zero Overhead):** High-impact modules (speech recognition, LLM, document analysis) remain completely idle/unloaded until explicitly woken.
2. **Local-First Routing:** Basic commands (e.g., "Open Chrome", "Mute Volume") bypass the LLM entirely, running via local rules/APIs (100–300 ms execution time).
3. **Cloud-on-Demand:** Only complex queries requiring reasoning, coding help, or document summarization are sent to the cloud LLM (Gemini/GPT).
4. **Skill-Based Extensibility:** Additional system integrations are implemented as modular "skills" that register specific intents.

---

## 2. Architecture Diagram

```
                     Dynamic Island UI
                             │
                     AI Orchestrator (Dumb Coordinator)
                             │
                   Push-to-Talk Manager (Ctrl + Space)
                             │
                      Audio Capture (Raw PCM)
                             │
                     Speech-to-Text (STT)
                             │
                      Intent Router (Local Rules)
                             │
                  Confidence & Context Manager
                             │
             ┌───────────────┴───────────────┐
             │                               │
       Local Skills                     LLM Service
  (System vs App Skills)                     │
             │                               │
     Action Planner                          │
             └───────────────┬───────────────┘
                             │
                      Windows Controller
                             │
                      Text-to-Speech (TTS)
                             │
                     Dynamic Island UI
```

---

## 3. Data Contracts & Type Definitions

To keep the system modular and testable, all components communicate via a common set of C++ data types. These are defined in `new_feature/contracts.h`.

### A. Intent Types
* **`Intent`:** The structured intent extracted from speech or text.
* **`IntentResult`:** Includes a confidence score, whether to fall back to the LLM, and the original text for logging/debugging.

### B. Execution Types
* **`Action`:** A single discrete Windows or system operation.
* **`ActionPlan`:** An ordered sequence of actions (allowing multi-step execution) along with the voice response text.
* **`SkillResult`:** The outcome of a skill execution, containing both audio feedback and screen text.

### C. AI Provider Types
* **`AIRequest` / `AIResponse`:** Vendor-agnostic message wrappers for chat prompts, code questions, STT, and TTS.

### D. Event System
* **`Event`:** The event model containing `EventType` (e.g., `VOICE_CAPTURED`, `STT_COMPLETED`, `COMMAND_EXECUTED`) and a payload.

---

## 4. The Event Bus Event Loop
Rather than hardcoding tight couplings (e.g., calling `TTS.Speak()` directly from the LLM service), components raise events on a central **Event Bus**:

1. **`VOICE_CAPTURED`** $\rightarrow$ Triggers the STT service.
2. **`STT_COMPLETED`** $\rightarrow$ Triggers the Intent Router.
3. **`INTENT_CLASSIFIED`** $\rightarrow$ Triggers the Skills Manager (local) or LLM (cloud).
4. **`COMMAND_EXECUTED`** / **`AI_RESPONSE_READY`** $\rightarrow$ Triggers TTS and Dynamic Island UI animations.
5. **`SPEECH_STARTED`** $\rightarrow$ Island displays talking/sound waves.
6. **`SPEECH_FINISHED`** $\rightarrow$ Island collapses back to idle.

---

## 5. Context Manager
A lightweight manager that stores session state to resolve relative commands:
* E.g., when the user says "Open Chrome", it sets `context.lastOpenedApp = "Chrome"`.
* When the user says "Close it", the Intent Router queries the Context Manager, resolves "it" to Chrome, and executes a close command on Chrome.

---

## 6. Budget-Friendly & Offline Implementation Stack (100% Free)

To ensure this project can be built without any operational costs or subscription requirements, we can implement the following stack:

### A. Speech-to-Text (STT) - Local & Free
* **Windows Native Speech Recognition:** We can use the Windows Media / Speech Recognition API (`Windows.Media.SpeechRecognition` via C++/WinRT) which runs entirely offline, utilizes the local Windows voice engine, and is **100% free with no API keys**.

### B. Text-to-Speech (TTS) - Local & Free
* **Windows SAPI / SpeechSynthesizer:** Windows has a built-in voice synthesizer (`Windows.Media.SpeechSynthesis::SpeechSynthesizer`) which is completely free, runs locally, and uses the default high-quality voices installed on Windows (like Microsoft David/Zira/Hazel).

### C. Intent Routing - Local & Free
* **Pattern Matching:** All system controller commands (e.g., volume, app opening, power options) are handled by local keyword matching rules, making them instant, completely free, and functional offline.

* **Phase 1: OpenRouter (Free Tier Gateway):** We will build and test using **OpenRouter** as our primary cloud AI provider using their OpenAI-compatible endpoint. We will utilize OpenRouter's **100% free models** (like `google/gemini-flash-1.5-free` or `meta-llama/llama-3-8b-instruct:free`). This requires zero payment and keeps the code ready for multi-model switching later.
* **Phase 2: Local AI & Direct Integration Options:** Once the core system is stable, we can add local offline LLM support (via Ollama) or direct API providers for advanced setups if desired.

---

## 7. Complete Detailed Implementation Roadmap

We will build the AI Voice Assistant incrementally in **8 testable milestones**. Each milestone will produce a working sub-component that can be verified in isolation.

```
Phase 1: Event Bus & Shared Models
   |
   v
Phase 2: WASAPI Audio Capture & PTT
   |
   v
Phase 3: local Whisper Speech-to-Text (STT)
   |
   v
Phase 4: Local Intent Router & Skills Manager
   |
   v
Phase 5: OpenRouter Configurable LLM
   |
   v
Phase 6: Windows TTS & Dynamic Island UI Animations
   |
   v
Phase 7: Session Context Manager & Memory
   |
   v
Phase 8: Plugin SDK (Third-party Skills)
```

### Milestone 1: The Event Bus & Orchestration Skeleton
* **Goal:** Set up the basic pub-sub communication system so components can send and receive messages asynchronously.
* **Deliverables:**
  * Implement the `EventBus` class in C++ to register event handlers and publish events.
  * Implement the `AIOrchestrator` base class to coordinate the event flow (subscribing to audio events, forwarding to routing, triggering executing and TTS).
* **Verification:** Run a simple test suite showing that publishing `VOICE_CAPTURED` triggers the orchestrator event loop without errors.

### Milestone 2: WASAPI Audio Capture & Push-to-Talk (PTT)
* **Goal:** Capture low-latency microphone audio using Windows Audio Session API (WASAPI) while holding `Ctrl + Space`.
* **Deliverables:**
  * Define the `IAudioCapture` interface and implement `WasapiAudioCapture` using native Windows WASAPI COM objects.
  * Listen for PTT events via a low-level keyboard hook or hotkey to trigger start/stop recording.
  * Output raw audio bytes.
* **Verification:** Hold `Ctrl + Space`, speak, release, and verify the audio buffer has captured data with low latency.

### Milestone 3: Speech-to-Text (STT) Service
* **Goal:** Translate voice recording into a text string using local Whisper recognition.
* **Deliverables:**
  * Define the `ISpeechToTextService` interface and implement `WhisperSTT` (e.g. leveraging `whisper.cpp` locally/offline).
  * Translate the audio buffer into a plain transcribed `std::wstring`.
* **Verification:** Speak "Open Notepad" offline and verify the transcription resolves to the correct string.

### Milestone 4: Local Intent Router & Skills Manager (Instant Actions)
* **Goal:** Execute basic Windows and app commands instantly and offline.
* **Deliverables:**
  * Implement `IntentRouter` pattern matching logic returning an `IntentResult` (with confidence, parameters, and `useLLM` toggle).
  * Group skills into `SystemSkill` (volume, power) and `AppSkill` (VS Code, Chrome, Spotify) implementing `ISkill`.
* **Verification:** Speak "Open Chrome", verify it bypasses network requests and launches Chrome within 200 ms.

### Milestone 5: OpenRouter LLM Integration (General Queries)
* **Goal:** Support general chat queries, coding help, and reasoning.
* **Deliverables:**
  * Implement the `IAIService` using OpenRouter's free endpoint (`https://openrouter.ai/api/v1/chat/completions`).
  * Make the target model configurable via `settings.ini` / `config.json` instead of hardcoding.
* **Verification:** Speak "Explain polymorphism", verify the response is fetched dynamically from the configured model.

### Milestone 6: Text-to-Speech (TTS) & Dynamic Island UI Animations (The Output)
* **Goal:** Complete the feedback loop with high-quality local voice synthesis and matching Direct2D animations.
* **Deliverables:**
  * Implement the `ITextToSpeechService` using Windows native voice synthesis (`Windows.Media.SpeechSynthesis`).
  * Add UI state handlers in Dynamic Island drawing loops to show:
    * Waveform visualizer for `Listening...`.
    * Thinking dot for `Processing...`.
    * Checkmark for `Done/Success`.
* **Verification:** A complete run: speak "Open Chrome", Chrome launches, the Island displays a success checkmark, and a native voice says "Chrome has been opened."

### Milestone 7: Context & Session Memory
* **Goal:** Resolve relative pronouns (like "it" or "that folder") based on recent interaction context.
* **Deliverables:**
  * Implement a local in-memory `ContextManager` tracking `lastOpenedApp`, `lastExecutedSkill`, and `lastAIResponse`.
  * Update `IntentRouter` to query `ContextManager` when relative pronouns are detected in incoming text.
* **Verification:** Speak "Open Chrome", then speak "Close it", verify that Chrome is closed successfully.

### Milestone 8: Plugin SDK & Third-party Skills
* **Goal:** Make the assistant highly extensible so users can add custom skills without editing the core orchestrator.
* **Deliverables:**
  * Expose the `ISkill` interface as a plugin SDK.
* **Verification:** Drop a custom `CustomSkill` file into a plugins directory and verify it is detected and successfully executed when its registered intent is spoken.

---

## 8. Version 1.0 Production Readiness & Advanced Future Enhancements

To take this AI Assistant from a validated core infrastructure (v0.1) into a polished, premium production release (v1.0), we will focus on these key areas and subsequent phases:

### A. Core Focus Areas for Version 1.0
1. **Production UI Integration:** Replace console/ASCII states with high-quality Direct2D rendering inside `src/window-dynamic-island.wh.cpp` (pulsating waveform for `Listening`, loading glow for `Processing`, checkmark for `Success`).
2. **Action Planner:** Transition from simple *Intent -> Execute* into *Intent -> Action Planner -> Sequential Execution*. E.g., *"I'm going to study"* resolves into:
   * Action 1: Open VS Code.
   * Action 2: Launch the target workspace folder.
   * Action 3: Start a coding focus timer.
3. **Streaming AI Responses:** Update `OpenRouterAIService` and the Event Bus to support streaming chunks (`EVENT_CHUNK_RECEIVED`) so voice/text rendering starts immediately without waiting for the complete LLM text.
4. **Stress Testing & Latency Optimization:** Validate memory boundaries over long PTT sessions, verify graceful recovery during network dropouts, and reuse HTTPS request handles to optimize API connection speeds.
5. **Real-world Security Review:** Confirm user prompt validations for high-impact actions (deleting files, shutdowns) requiring a visual click confirmation state.

### B. Future Roadmap (Version 2.0+)
* **Phase 9 (Vision & Multimodal):** Intercept screen captures (`Ctrl + Shift + A`) and route screenshots to OpenRouter's vision models to solve coding bugs, translate graphics, or explain on-screen text.
* **Phase 10 (Proactive System Intelligence):** Set background observers to monitor battery (e.g., offer battery saver at 12%), Bluetooth (AirPods connectionpopups), and productivity timers (stop timer on IDE exit).
* **Phase 11 (Persistent Memory):** Remember preferred defaults (favourite browser, project workspace paths) to personalize local execution.
* **Phase 12 (Dynamic Model Manager):** Auto-select the best model for the task (DeepSeek for coding, Gemini for fast general answers) while allowing user customization.

