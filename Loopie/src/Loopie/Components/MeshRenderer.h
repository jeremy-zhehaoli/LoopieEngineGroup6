#pragma once
#include "Loopie/Resources/Types/Mesh.h"
#include "Loopie/Render/Shader.h"
#include "Loopie/Render/Texture.h"


#include <memory>

namespace Loopie {
	class MeshRenderer {
	public:
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Mesh> mesh);
		~MeshRenderer() = default;

		void Render();
		Shader& GetShader() { return m_shader; }
		std::shared_ptr<Mesh> GetMesh() { return m_mesh; }
	private:

		Shader m_shader = Shader("assets/shaders/CorrectShader.shader");
		Texture m_texture = Texture("assets/texture.png");
		std::shared_ptr<Mesh> m_mesh;
	};
}