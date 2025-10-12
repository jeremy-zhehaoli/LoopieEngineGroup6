#pragma once

#include "Loopie/Resources/Types/Mesh.h"

#include <memory>
#include <string>

namespace Loopie {
	class MeshImporter {
	public:
		static std::vector<std::shared_ptr<Mesh>> LoadModel(const std::string& filepath);
		static bool CheckIfIsModel(const char* path);

	private:
		static void ProcessNode(void* node, const void* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
		static std::shared_ptr<Mesh> ProcessMesh(void* mesh, const void* scene);
	};
}