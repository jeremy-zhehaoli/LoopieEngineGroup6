#pragma once
#include "Loopie/Core/Math.h"

#include <memory>

namespace Loopie {

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	class Gizmo {
		friend class Renderer;
	public:
		static void Init();
		static void Shutdown();

		static void DrawLine(const vec3& p0, const vec3& p1, const vec4& color = {1,1,1,1});

	private:

		static void BeginGizmo(const matrix4 viewProjectionMatrix);
		static void EndGizmo();

		static void Flush();
		static void StartBatch();
		static void NextBatch();

		struct LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct GizmoTypeRender {
			std::shared_ptr<VertexArray> VertexArray;
			std::shared_ptr<VertexBuffer> VertexBuffer;
			std::shared_ptr<IndexBuffer> IndexBuffer;

			std::shared_ptr <Shader> Shader;
		};

		struct GizmoData
		{
			const unsigned int MAX_LINES = 10000;
			const unsigned int MAX_VERTICES = MAX_LINES * 2;
			const float LINE_WIDTH = 2.0f;

			/// Lines
			GizmoTypeRender LineRender;
			LineVertex* LineBufferModel = nullptr;
			LineVertex* LineBuffer = nullptr;
			unsigned int LineCount = 0;
			///

			matrix4 ViewProjection = matrix4(1);
		};

	private:
		static GizmoData s_Data;
	};
}