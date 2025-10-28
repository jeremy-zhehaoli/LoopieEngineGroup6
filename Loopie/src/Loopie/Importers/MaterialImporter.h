#pragma once
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Resources/MetadataRegistry.h"

#include <memory>
#include <vector>
#include <string>


namespace Loopie {
	class MaterialImporter {
	public:
		static void ImportMaterial(const std::string& filepath, Metadata& metadata);
		static void LoadMaterial(const std::string& path, Material& material);
		static bool CheckIfIsMaterial(const char* path);
	private:

	};
}