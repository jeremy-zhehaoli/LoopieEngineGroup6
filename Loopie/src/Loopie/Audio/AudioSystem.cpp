#include "AudioSystem.h"
#include "Loopie/Core/Log.h"
#include <filesystem>
#include <algorithm>

namespace Loopie {
	AudioSystem::AudioSystem()
		: m_system(nullptr)
		, m_track1(nullptr)
		, m_track2(nullptr)
		, m_currentChannel(nullptr)
		, m_currentTrack(0)
		, m_isPlaying(false)
		, m_isInitialized(false)
	{
	}

	AudioSystem::~AudioSystem()
	{
		Shutdown();
	}

	bool AudioSystem::Initialize()
	{
		if (m_isInitialized)
		{
			Log::Warn("AudioSystem already initialized");
			return true;
		}

		FMOD_RESULT result = FMOD::System_Create(&m_system);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to create FMOD system: {0}", result);
			return false;
		}

		result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to initialize FMOD system: {0}", result);
			m_system->release();
			m_system = nullptr;
			return false;
		}

		m_isInitialized = true;
		Log::Info("AudioSystem initialized successfully");
		return true;
	}

	void AudioSystem::Shutdown()
	{
		if (!m_isInitialized)
			return;

		StopBackgroundMusic();

		if (m_track1)
		{
			m_track1->release();
			m_track1 = nullptr;
		}

		if (m_track2)
		{
			m_track2->release();
			m_track2 = nullptr;
		}

		if (m_system)
		{
			m_system->close();
			m_system->release();
			m_system = nullptr;
		}

		m_isInitialized = false;
		Log::Info("AudioSystem shut down");
	}

	void AudioSystem::Update()
	{
		if (!m_isInitialized || !m_system)
			return;

		m_system->update();
		CheckAndSwitchTracks();
	}

	bool AudioSystem::LoadBackgroundMusic(const std::string& track1Path, const std::string& track2Path)
	{
		if (!m_isInitialized)
		{
			Log::Error("AudioSystem not initialized. Call Initialize() first.");
			return false;
		}

		if (m_track1)
		{
			m_track1->release();
			m_track1 = nullptr;
		}
		if (m_track2)
		{
			m_track2->release();
			m_track2 = nullptr;
		}

		if (!std::filesystem::exists(track1Path))
		{
			Log::Error("Background music track 1 not found: {0}", track1Path);
			return false;
		}
		if (!std::filesystem::exists(track2Path))
		{
			Log::Error("Background music track 2 not found: {0}", track2Path);
			return false;
		}

		FMOD_RESULT result = m_system->createSound(track1Path.c_str(), FMOD_CREATESTREAM | FMOD_LOOP_OFF, nullptr, &m_track1);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to load background music track 1: {0}", result);
			return false;
		}

		result = m_system->createSound(track2Path.c_str(), FMOD_CREATESTREAM | FMOD_LOOP_OFF, nullptr, &m_track2);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to load background music track 2: {0}", result);
			m_track1->release();
			m_track1 = nullptr;
			return false;
		}

		Log::Info("Background music tracks loaded successfully");
		return true;
	}

	void AudioSystem::PlayBackgroundMusic()
	{
		if (!m_isInitialized || !m_system)
		{
			Log::Warn("AudioSystem not initialized");
			return;
		}

		if (!m_track1 || !m_track2)
		{
			Log::Warn("Background music tracks not loaded");
			return;
		}

		if (m_isPlaying)
		{
			Log::Warn("Background music is already playing");
			return;
		}

		m_currentTrack = 0;
		FMOD::Sound* soundToPlay = m_currentTrack == 0 ? m_track1 : m_track2;
		
		FMOD_RESULT result = m_system->playSound(soundToPlay, nullptr, false, &m_currentChannel);
		if (result != FMOD_OK)
		{
			Log::Error("Failed to play background music: {0}", result);
			return;
		}

		m_isPlaying = true;
		Log::Info("Background music started");
	}

	void AudioSystem::StopBackgroundMusic()
	{
		if (!m_isPlaying || !m_currentChannel)
			return;

		FMOD_RESULT result = m_currentChannel->stop();
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE)
		{
			Log::Warn("Error stopping background music: {0}", result);
		}

		m_currentChannel = nullptr;
		m_isPlaying = false;
		m_currentTrack = 0;
	}

	void AudioSystem::PauseBackgroundMusic(bool pause)
	{
		if (!m_currentChannel)
			return;

		FMOD_RESULT result = m_currentChannel->setPaused(pause);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE)
		{
			Log::Warn("Error pausing/unpausing background music: {0}", result);
		}
	}

	bool AudioSystem::IsBackgroundMusicPlaying() const
	{
		if (!m_currentChannel)
			return false;

		bool playing = false;
		FMOD_RESULT result = m_currentChannel->isPlaying(&playing);
		if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE)
		{
			return false;
		}

		return playing && m_isPlaying;
	}

	void AudioSystem::SetMasterVolume(float volume)
	{
		if (!m_system)
			return;

		volume = std::max(0.0f, std::min(1.0f, volume));

		FMOD::ChannelGroup* masterGroup = nullptr;
		FMOD_RESULT result = m_system->getMasterChannelGroup(&masterGroup);
		if (result == FMOD_OK && masterGroup)
		{
			masterGroup->setVolume(volume);
		}
	}

	float AudioSystem::GetMasterVolume() const
	{
		if (!m_system)
			return 0.0f;

		FMOD::ChannelGroup* masterGroup = nullptr;
		FMOD_RESULT result = m_system->getMasterChannelGroup(&masterGroup);
		if (result == FMOD_OK && masterGroup)
		{
			float volume = 0.0f;
			masterGroup->getVolume(&volume);
			return volume;
		}

		return 0.0f;
	}

	void AudioSystem::CheckAndSwitchTracks()
	{
		if (!m_isPlaying || !m_currentChannel)
			return;

		bool playing = false;
		FMOD_RESULT result = m_currentChannel->isPlaying(&playing);
		
		if (result == FMOD_OK && !playing)
		{
			m_currentTrack = (m_currentTrack == 0) ? 1 : 0;
			FMOD::Sound* soundToPlay = m_currentTrack == 0 ? m_track1 : m_track2;

			result = m_system->playSound(soundToPlay, nullptr, false, &m_currentChannel);
			if (result != FMOD_OK)
			{
				Log::Error("Failed to switch background music track: {0}", result);
				m_isPlaying = false;
				m_currentChannel = nullptr;
			}
			else
			{
				Log::Info("Switched to background music track {0}", m_currentTrack + 1);
			}
		}
		else if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE)
		{
			m_isPlaying = false;
			m_currentChannel = nullptr;
		}
	}
}
