#pragma once

#include "Loopie/Core/Module.h"
#include "Loopie/Core/Log.h"

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <string>
#include <memory>

namespace Loopie {
	class AudioManager : public Module {
	public:
		AudioManager();
		~AudioManager();

		void OnLoad() override;
		void OnUnload() override;
		void OnUpdate() override;

		FMOD::Studio::System* GetStudioSystem() { return m_studioSystem; }
		FMOD::System* GetCoreSystem() { return m_coreSystem; }

		FMOD::Sound* CreateSound(const std::string& filePath, FMOD_MODE mode = FMOD_DEFAULT);
		void PlaySound(FMOD::Sound* sound, FMOD::Channel** channel = nullptr, bool paused = false);
		
		void Update();

		static AudioManager& GetInstance();

	private:
		void InitializeFMOD();
		void ShutdownFMOD();

		FMOD::Studio::System* m_studioSystem = nullptr;
		FMOD::System* m_coreSystem = nullptr;
		static AudioManager* s_Instance;
	};
}