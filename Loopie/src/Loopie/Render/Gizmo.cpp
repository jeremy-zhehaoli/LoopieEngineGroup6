#include "Gizmo.h"

#include "Loopie/Core/Assert.h"

#include "Loopie/Render/Shader.h"
#include "Loopie/Render/VertexArray.h"
#include "Loopie/Render/VertexBuffer.h"
#include "Loopie/Render/IndexBuffer.h"

#include <glad/glad.h>

namespace Loopie {
	
	Gizmo::GizmoData Gizmo::s_Data;

	void Gizmo::Init() {
		s_Data.LineRender.VertexArray = std::make_shared<VertexArray>();
		s_Data.LineRender.VertexBuffer = std::make_shared<VertexBuffer>(nullptr, s_Data.MAX_VERTICES * sizeof(LineVertex));

		BufferLayout& layout = s_Data.LineRender.VertexBuffer->GetLayout();
		layout.AddLayoutElement(0, GLVariableType::FLOAT, 3, "a_Position");
		layout.AddLayoutElement(4, GLVariableType::FLOAT, 4, "a_Color");

		unsigned int* indices = new uint32_t[s_Data.MAX_LINES * 2];
		for (uint32_t i = 0; i < s_Data.MAX_LINES * 2; i++)
			indices[i] = i;
		s_Data.LineRender.IndexBuffer = std::make_shared<IndexBuffer>(indices, s_Data.MAX_LINES * 2);
		delete[] indices;

		s_Data.LineRender.VertexArray->AddBuffer(s_Data.LineRender.VertexBuffer.get(), s_Data.LineRender.IndexBuffer.get());

		s_Data.LineBufferModel = new LineVertex[s_Data.MAX_VERTICES];

		s_Data.LineRender.Shader = std::make_shared<Shader>("assets/shaders/LineShader.shader");
	}

	void Gizmo::Shutdown() {
		delete s_Data.LineBufferModel;
	}

	void Gizmo::BeginGizmo(const matrix4 viewProjectionMatrix)
	{
		s_Data.ViewProjection = viewProjectionMatrix;

		s_Data.LineRender.Shader->Bind();
		s_Data.LineRender.Shader->SetUniformMat4("u_ViewProjection", s_Data.ViewProjection);

		StartBatch();
	}

	void Gizmo::EndGizmo()
	{
		unsigned int dataSize = (uint32_t)((uint8_t*)s_Data.LineBuffer - (uint8_t*)s_Data.LineBufferModel);
		s_Data.LineRender.VertexBuffer->SetData(s_Data.LineBufferModel, dataSize);

		Flush();
	}

	void Gizmo::DrawLine(const vec3& p0, const vec3& p1, const vec4& color)
	{
		if (s_Data.LineCount >= s_Data.MAX_LINES)
			NextBatch();

		s_Data.LineBuffer->Position = p0;
		s_Data.LineBuffer->Color = color;
		s_Data.LineBuffer++;

		s_Data.LineBuffer->Position = p1;
		s_Data.LineBuffer->Color = color;
		s_Data.LineBuffer++;

		s_Data.LineCount++;

	}

	void Gizmo::Flush() {
		if (s_Data.LineCount == 0)
			return;

		s_Data.LineRender.VertexArray->Bind();
		s_Data.LineRender.Shader->Bind();
		glDrawElements(GL_LINES, s_Data.LineCount * 2, GL_UNSIGNED_INT, nullptr);
		s_Data.LineRender.VertexArray->Unbind();
	}

	void Gizmo::StartBatch()
	{
		s_Data.LineCount = 0;
		s_Data.LineBuffer = s_Data.LineBufferModel;
	}

	void Gizmo::NextBatch()
	{
		EndGizmo();
		StartBatch();
	}
}