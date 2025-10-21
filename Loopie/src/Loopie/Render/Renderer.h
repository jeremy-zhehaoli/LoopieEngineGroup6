#pragma once
#include "Loopie/Core/Math.h"
#include "Loopie/Render/Shader.h"
#include "Loopie/Render/VertexArray.h"

#include <filesystem>

namespace Loopie {
	class Transform;

	class Renderer {
	public:
		static void Init(void* context);
		static void Shutdown();

		static void Clear();
		static void SetClearColor(vec4 color);
		static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		static void BeginScene(const matrix4 viewProjectionMatrix);
		static void EndScene();

		static void Draw(std::shared_ptr<VertexArray> vao, Shader& shader, const Transform* transform);
	};
}