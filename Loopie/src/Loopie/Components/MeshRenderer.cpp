#include "MeshRenderer.h"

#include "Loopie/Render/Renderer.h"

namespace Loopie {

	MeshRenderer::MeshRenderer() {
		m_shader.Bind();
		m_texture.Bind();
		m_shader.SetUniformInt("u_Texture", 0);
		m_shader.Unbind();
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh) : MeshRenderer()
	{
		m_mesh = mesh;
	}
	void MeshRenderer::Render() {
		if(m_mesh)
			Renderer::Draw(m_mesh->m_vao, m_shader);
	}
}