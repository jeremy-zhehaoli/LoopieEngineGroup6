#pragma once

#include "fmod.hpp"
#include <string>
#include <memory>

namespace Loopie {
	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();

		bool Initialize();
		void Shutdown();
		void Update();

		bool LoadBackgroundMusic(const std::string& track1Path, const std::string& track2Path);
		void PlayBackgroundMusic();
		void StopBackgroundMusic();
		void PauseBackgroundMusic(bool pause);
		bool IsBackgroundMusicPlaying() const;

		void SetMasterVolume(float volume);
		float GetMasterVolume() const;

	private:
		void CheckAndSwitchTracks();

		FMOD::System* m_system;
		FMOD::Sound* m_track1;
		FMOD::Sound* m_track2;
		FMOD::Channel* m_currentChannel;
		
		int m_currentTrack;
		bool m_isPlaying;
		bool m_isInitialized;
	};
}
