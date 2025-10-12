#pragma once
#include "Loopie/Resources/Resource.h"

#include "Loopie/Render/IndexBuffer.h"
#include "Loopie/Render/VertexBuffer.h"
#include "Loopie/Render/VertexArray.h"
#include <vector>

namespace Loopie {

	struct VertexComponents {
		bool Position = true;
		bool TexCoord = false;
		bool Tangent = false;
		bool Color = false;
		bool Normal = false;
	};

	class Mesh /*: public Resource*/{
		friend class MeshRenderer;
	public :
		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexComponents& components);
		~Mesh() = default;

		/*void LoadFromFile(const std::string path) override;
		void Reload() override;*/
	private:
		std::vector<float> m_vertices;
		std::vector<unsigned int> m_indices;
		VertexComponents m_components;

		VertexArray m_vao;
		VertexBuffer m_vbo;
		IndexBuffer m_ebo;

	};
}