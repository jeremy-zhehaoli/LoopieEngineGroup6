# LoopieEngineGroup6

**GROUP MEMBERS:**

Zhehao Li - Link to Github: https://github.com/jeremy-zhehaoli

Victor Cardenas Hormigo - Link to GitHub: https://github.com/vixtorbir

Marc Amo Mateu - Link to GitHub: https://github.com/mxarc

Link to the repository - https://github.com/jeremy-zhehaoli/LoopieEngineGroup6

## Engine Description

LoopieEngine is a project for the subject of Game Engines in our 3rd year at CITM (UPC) made by a group of our class. We had to fork their engine for the final project, where we had to add the AudioEngine as we chose.

## IMPORTANT:
If you want to add different audio files to test how it works, put them in the Assets folder of the project you create in the path you will have chosen.

## Sub-system main core

The main core sub-systems of a game audio engine are centered around to create an immersive environment by using AudioSources, an InteractiveMusic system and Basic Audio filters like Doppler, Reverb, FallOff...

## Sub-system
Audio System (FMOD Integration)

A robust, component-based audio sub-system powered by the *FMOD Studio API*. The system is fully integrated into the engine's ECS (Entity Component System), supporting 3D spatialization, dynamic environmental effects, and state persistence.

### Architecture

The system follows a decoupled architecture separating the core logic from the scene components:

* *AudioManager (Core):* A static singleton managing the FMOD system, global updates, and environmental rules (Tunnel Zones).
* *AudioSource (Component):* Autonomous component handling playback state, 3D positioning updates, and playlist management.
* *AudioListener (Component):* Defines the spatial "ears" of the scene (typically attached to the Camera), reporting position and orientation to the engine.

### Key Features

* *3D Spatial Audio:* Real-time attenuation (Linear Rolloff) and panning based on the relative position between the Source and the Listener.
* *Dynamic "Tunnel Effect":* An interactive environmental mechanic using *AABB (Axis-Aligned Bounding Box)* detection.
* The AudioManager defines a dynamic zone.
* When an AudioSource enters this zone, its *pitch is automatically shifted to 0.5f* (slow-motion effect), restoring to 1.0f upon exit.
* *Playlist System:* Support for queuing multiple audio clips in a single source with auto-advance logic.
* *Serialization System:* Full JSON persistence support. Audio properties (Volume, Loop, PlayOnAwake, Playlist) are saved and loaded with the scene.
* *Editor Integration:*
* Custom Inspector UI for dragging & dropping audio files.
* Visual debugging helpers (primitive shapes) for sources and listeners.

### Build System (CMake)

Automated dependency management using *CMake Custom Commands*. The build script automatically detects the architecture (x64/x86) and copies the required FMOD DLLs (fmod.dll, fmodstudio.dll) to the output executable directory via a POST_BUILD event, ensuring a plug-and-play experience.

![Gif1](https://github.com/user-attachments/assets/19371394-f3b1-4a7f-8c89-54918d607cf3)
![Gif2](https://github.com/user-attachments/assets/b1cb0bd1-d49b-4cfe-b7f8-313ba3f4f616)

[Video](https://youtu.be/V1sXxb5-mRw)
