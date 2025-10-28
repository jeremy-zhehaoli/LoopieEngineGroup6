#pragma once
#include "Loopie/Resources/Types/Texture.h"
#include "Loopie/Resources/MetadataRegistry.h"

#include <memory>
#include <vector>
#include <string>

namespace Loopie {
	class TextureImporter {
	public:
		static void ImportImage(const std::string& filepath, Metadata& metadata);
		static void LoadImage(const std::string& filepath, Texture& texture);
		static bool CheckIfIsImage(const char* path);
	};
}