#include "MeshRenderer.h"

#include "Loopie/Render/Renderer.h"
#include "Loopie/Render/Gizmo.h"
#include "Loopie/Core/Math.h"
#include "Loopie/Components/Transform.h"

namespace Loopie {

	MeshRenderer::MeshRenderer() {
		
	}

	void MeshRenderer::Render() {
		if (m_mesh) {
			Renderer::Draw(m_mesh->m_vao, m_material, GetTransform());
			///TEST
			if(m_drawNormalsPerFace)
				RenderNormalsPerFace(0.5f,{0,1,1,1});
			if(m_drawNormalsPerTriangle)
				RenderNormalsPerTriangle(0.5f,{1,1,0,1});
			///TEST
		}
		
	}

	void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
	}

	void MeshRenderer::SetMaterial(std::shared_ptr<Material> material)
	{
		m_material = material;
	}

	void MeshRenderer::Init()
	{
		m_material = std::make_shared<Material>();
	}
	

	///TEST
	vec3 MeshRenderer::GetVertexVec3Data(const MeshData& data, unsigned int vertexIndex, unsigned int offset)
	{
		vec3 vec3Data(0.0f);

		unsigned int base = vertexIndex * data.VertexElements + offset / sizeof(float);
		if (base + 2 < data.Vertices.size()) {
			vec3Data.x = data.Vertices[base + 0];
			vec3Data.y = data.Vertices[base + 1];
			vec3Data.z = data.Vertices[base + 2];
		}
		return vec3Data;
	};

	void MeshRenderer::RenderNormalsPerFace(float length, const vec4& color) {
		MeshData& data = m_mesh->m_data;
		if (data.VerticesAmount == 0 || data.IndicesAmount == 0)
			return;

		const BufferLayout& layout = m_mesh->m_vbo->GetLayout();
		BufferElement posElem = layout.GetElementByIndex(0); // a_Position
		if (posElem.Type == GLVariableType::NONE)
			return;

		Transform* transform = GetTransform();
		vec3 transformPos = +transform->GetPosition();
		for (unsigned int i = 0; i + 5 < data.Indices.size(); i += 6) {
			unsigned int indices[6] = {
				data.Indices[i + 0], data.Indices[i + 1], data.Indices[i + 2],
				data.Indices[i + 3], data.Indices[i + 4], data.Indices[i + 5]
			};

			bool skip = false;
			for (int j = 0; j < 6; j++)
				if (indices[j] >= data.VerticesAmount)
					skip = true;
			if (skip) continue;

			vec3 p0 = GetVertexVec3Data(data, indices[0], posElem.Offset);
			vec3 p1 = GetVertexVec3Data(data, indices[1], posElem.Offset);
			vec3 p2 = GetVertexVec3Data(data, indices[2], posElem.Offset);
			vec3 p3 = GetVertexVec3Data(data, indices[3], posElem.Offset);
			vec3 p4 = GetVertexVec3Data(data, indices[4], posElem.Offset);
			vec3 p5 = GetVertexVec3Data(data, indices[5], posElem.Offset);

			vec3 n1 = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			vec3 n2 = glm::normalize(glm::cross(p4 - p3, p5 - p3));

			vec3 faceNormal = glm::normalize((n1 + n2) * 0.5f);

			vec3 centroid1 = (p0 + p1 + p2) / 3.0f;
			vec3 centroid2 = (p3 + p4 + p5) / 3.0f;
			vec3 faceCentroid = (centroid1 + centroid2) * 0.5f;

			Gizmo::DrawLine(faceCentroid + transformPos, faceCentroid + transformPos + faceNormal * length, color);
		}
	}

	void MeshRenderer::RenderNormalsPerTriangle(float length, const vec4& color)
	{
		MeshData& data = m_mesh->m_data;
		if (data.VerticesAmount == 0 || data.IndicesAmount == 0)
			return;

		const BufferLayout& layout = m_mesh->m_vbo->GetLayout();
		BufferElement posElem = layout.GetElementByIndex(0); // a_Position
		if (posElem.Type == GLVariableType::NONE)
			return;

		Transform* transform = GetTransform();
		vec3 transformPos = +transform->GetPosition();
		for (unsigned int i = 0; i + 2 < data.Indices.size(); i += 3) {
			unsigned int index0 = data.Indices[i + 0];
			unsigned int index1 = data.Indices[i + 1];
			unsigned int index2 = data.Indices[i + 2];

			if (index0 >= data.VerticesAmount || index1 >= data.VerticesAmount || index2 >= data.VerticesAmount)
				continue;

			vec3 p0 = GetVertexVec3Data(data, index0, posElem.Offset);
			vec3 p1 = GetVertexVec3Data(data, index1, posElem.Offset);
			vec3 p2 = GetVertexVec3Data(data, index2, posElem.Offset);

			vec3 n = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			vec3 centroid = (p0 + p1 + p2) / 3.0f;

			Gizmo::DrawLine(centroid + transformPos, centroid + transformPos + n * length, color);
		}
	}
	///
}