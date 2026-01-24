#include "AudioManager.h"

#include "Loopie/Core/Assert.h"
#include "Loopie/Core/Log.h"
#include <fmod_errors.h>

namespace Loopie {
	AudioManager* AudioManager::s_Instance = nullptr;

	AudioManager::AudioManager() : Module("AudioManager", true)
	{
		ASSERT(s_Instance != nullptr, "AudioManager instance already exists");
		s_Instance = this;
	}

	AudioManager::~AudioManager()
	{
		ShutdownFMOD();
		s_Instance = nullptr;
	}

	void AudioManager::OnLoad()
	{
		InitializeFMOD();
	}

	void AudioManager::OnUnload()
	{
		ShutdownFMOD();
	}

	void AudioManager::OnUpdate()
	{
		Update();
	}

	void AudioManager::InitializeFMOD()
	{
		FMOD_RESULT result;

		result = FMOD::System_Create(&m_coreSystem);
		if (result != FMOD_OK)
		{
			Log::Error("FMOD Core System creation failed: {0}", FMOD_ErrorString(result));
			return;
		}

		result = m_coreSystem->init(512, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			Log::Error("FMOD Core System initialization failed: {0}", FMOD_ErrorString(result));
			return;
		}

		result = FMOD::Studio::System::create(&m_studioSystem);
		if (result != FMOD_OK)
		{
			Log::Error("FMOD Studio System creation failed: {0}", FMOD_ErrorString(result));
			return;
		}

		result = m_studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			Log::Error("FMOD Studio System initialization failed: {0}", FMOD_ErrorString(result));
			return;
		}

		Log::Info("FMOD Audio System initialized successfully");
	}

	void AudioManager::ShutdownFMOD()
	{
		if (m_studioSystem)
		{
			m_studioSystem->release();
			m_studioSystem = nullptr;
		}

		if (m_coreSystem)
		{
			m_coreSystem->release();
			m_coreSystem = nullptr;
		}

		Log::Info("FMOD Audio System shutdown");
	}

	void AudioManager::Update()
	{
		if (m_studioSystem)
		{
			m_studioSystem->update();
		}
		if (m_coreSystem)
		{
			m_coreSystem->update();
		}
	}

	FMOD::Sound* AudioManager::CreateSound(const std::string& filePath, FMOD_MODE mode)
	{
		if (!m_coreSystem)
		{
			Log::Error("FMOD Core System not initialized");
			return nullptr;
		}

		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = m_coreSystem->createSound(filePath.c_str(), mode, nullptr, &sound);
		
		if (result != FMOD_OK)
		{
			Log::Error("Failed to create sound from file: {0}, Error: {1}", filePath, FMOD_ErrorString(result));
			return nullptr;
		}

		return sound;
	}

	void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel, bool paused)
	{
		if (!m_coreSystem || !sound)
		{
			Log::Error("FMOD Core System not initialized or sound is null");
			return;
		}

		FMOD_RESULT result = m_coreSystem->playSound(sound, nullptr, paused, channel);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to play sound: {0}", FMOD_ErrorString(result));
		}
	}

	AudioManager& AudioManager::GetInstance()
	{
		ASSERT(s_Instance != nullptr, "AudioManager instance is null");
		return *s_Instance;
	}
}