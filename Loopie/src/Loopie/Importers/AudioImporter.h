#pragma once

#include "Loopie/Resources/Types/AudioClip.h"
#include "Loopie/Resources/MetadataRegistry.h"

#include <string>

namespace Loopie {
	class AudioImporter {
	public:
		static void ImportAudio(const std::string& filepath, Metadata& metadata);
		static void LoadAudio(const std::string& path, AudioClip& clip);
		static bool CheckIfIsAudio(const char* path);
	};
}