#pragma once
#include "Loopie/Core/Math.h"
#include "Loopie/Components/Transform.h"
namespace Loopie
{
	class Camera
	{
	public:
		Camera(Transform& transform, float fov = 45.0f, float near_plane = 0.1f, float far_plane = 200.0f);
		~Camera() = default;

		void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		const matrix4& GetViewMatrix()const;
		const matrix4& GetProjectionMatrix()const;
		const matrix4& GetViewProjectionMatrix()const;

	private:
		void CalculateMatrices();
	public:
		Transform& m_transform;

	private:
		vec4 m_viewport;

		float m_fov;
		float m_near_plane;
		float m_far_plane;


		matrix4 m_viewMatrix;
		matrix4 m_projectionMatrix;
		matrix4 m_viewProjectionMatrix;
	};
}