#include "Camera.h"

namespace Loopie
{
	Camera::Camera(Transform& transform, float fov, float near_plane, float far_plane): m_transform(transform), m_fov(fov), m_near_plane(near_plane), m_far_plane(far_plane)
	{
		m_transform.OnTransformDirty = [this]() {CalculateMatrices(); };
	}

	void Camera::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		m_viewport = vec4(x, y, width, height);
		CalculateMatrices();
	}

	const matrix4& Camera::GetViewMatrix() const
	{
		return m_viewMatrix;
	}

	const matrix4& Camera::GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	const matrix4& Camera::GetViewProjectionMatrix() const
	{
		return m_viewProjectionMatrix;
	}


	void Loopie::Camera::CalculateMatrices()
	{
		m_viewMatrix = m_transform.GetTransformMatrix();
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_viewport.z / m_viewport.w, m_near_plane, m_far_plane);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}