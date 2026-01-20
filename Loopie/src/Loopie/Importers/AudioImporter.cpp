#include "AudioImporter.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Files/DirectoryManager.h"

#include <cctype>
#include <filesystem>
#include <unordered_set>

namespace Loopie {
	namespace {
		std::string ToLower(std::string value)
		{
			for (char& c : value) {
				c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			}
			return value;
		}

		bool IsAudioExtension(const std::string& extension)
		{
			static const std::unordered_set<std::string> kAudioExtensions = {
				".wav",
				".ogg",
				".mp3",
				".flac",
				".aiff",
				".aif",
				".wem",
				".bnk"
			};
			return kAudioExtensions.count(ToLower(extension)) > 0;
		}
	}

	void AudioImporter::ImportAudio(const std::string& filepath, Metadata& metadata)
	{
		if (metadata.HasCache && !metadata.IsOutdated)
			return;

		std::filesystem::path sourcePath = filepath;
		std::string extension = sourcePath.extension().string();
		if (!IsAudioExtension(extension)) {
			Log::Warn("Unsupported audio format for import: {0}", filepath);
			return;
		}

		Project project = Application::GetInstance().m_activeProject;
		DirectoryManager::CreateFolder(project.GetChachePath(), "Audio");

		UUID id;
		std::filesystem::path locationPath = "Audio";
		locationPath /= id.Get() + ToLower(extension);
		std::filesystem::path pathToWrite = project.GetChachePath() / locationPath;

		std::error_code error;
		std::filesystem::copy_file(sourcePath, pathToWrite, std::filesystem::copy_options::overwrite_existing, error);
		if (error) {
			Log::Error("Failed to import audio {0}: {1}", filepath, error.message());
			return;
		}

		metadata.HasCache = true;
		metadata.CachesPath.clear();
		metadata.CachesPath.push_back(locationPath.string());
		metadata.Type = ResourceType::AUDIO;

		MetadataRegistry::SaveMetadata(filepath, metadata);

		Log::Trace("Audio Imported -> {0}", filepath);
	}

	void AudioImporter::LoadAudio(const std::string& path, AudioClip& clip)
	{
		Project project = Application::GetInstance().m_activeProject;
		std::filesystem::path filepath = project.GetChachePath() / path;
		if (!std::filesystem::exists(filepath)) {
			Log::Warn("Audio cache file not found: {0}", filepath.string());
			return;
		}

		clip.m_cachePath = path;
		clip.m_absolutePath = filepath.string();
	}

	bool AudioImporter::CheckIfIsAudio(const char* path)
	{
		if (!path)
			return false;
		std::filesystem::path filePath = path;
		return IsAudioExtension(filePath.extension().string());
	}
}