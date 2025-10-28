#pragma once
#include "Loopie/Resources/Types/Mesh.h"
#include "Loopie/Resources/MetadataRegistry.h"

#include <memory>
#include <vector>
#include <string>


namespace Loopie {
	class MeshImporter {
	public:
		static void ImportModel(const std::string& filepath, Metadata& metadata);
		static void LoadModel(const std::string& path, Mesh& mesh);
		static bool CheckIfIsModel(const char* path);

	private:
		static void ProcessNode(void* node, const void* scene, std::vector<std::string>& outputPaths);
		static std::string ProcessMesh(void* mesh, const void* scene);
	};
}