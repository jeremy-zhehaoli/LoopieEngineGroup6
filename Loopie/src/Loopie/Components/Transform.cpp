#include "Transform.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Scene/Entity.h"

namespace Loopie
{
    Transform::Transform(const vec3& position, const quaternion& rotation, const vec3& scale)
        : m_position(position), m_rotation(rotation), m_scale(scale)
    {
        SetDirty();
    }

    void Transform::Init()
    {}

#pragma region Transform Matrix
    matrix4 Transform::GetTransformMatrix() const
    {
        RecalculateCache();
        return m_matrix;
    }

    vec3 Transform::WorldToLocalPoint(const vec3& localPoint) const
    {
        return GetTransformMatrix() * glm::vec4(localPoint, 1.0f);
    }

    vec3 Transform::LocalToWorldPoint(const vec3& worldPoint) const
    {
        return glm::inverse(GetTransformMatrix()) * glm::vec4(worldPoint, 1.0f);
    }
#pragma endregion

#pragma region Position
    const vec3& Transform::GetPosition() const
    {
        return m_position;
    }

    void Transform::SetPosition(const vec3& position)
    {
        m_position = position;
        SetDirty();
    }

    void Transform::Translate(const vec3& translation, bool localSpace)
    {
        if (localSpace)
            m_position += m_rotation * translation;
        else
            m_position += translation;
        SetDirty();
    }
#pragma endregion

#pragma region Rotation
    quaternion Transform::QuaternionGetRotation() const
    {
        return m_rotation;
    }

    vec3 Transform::DegreesGetEulerAngles() const
    {
        return glm::degrees(glm::eulerAngles(m_rotation));
    }

    vec3 Transform::RadiansGetEulerAngles() const
    {
        return glm::eulerAngles(m_rotation);
    }

    void Transform::QuaternionSetRotation(const quaternion& rotation)
    {
        m_rotation = rotation;
        SetDirty();
    }

    void Transform::DegreesSetRotation(const vec3& degrees)
    {
        m_rotation = glm::quat(glm::radians(degrees));
        SetDirty();
    }

    void Transform::RadiansSetRotation(const vec3& radians)
    {
        m_rotation = glm::quat(radians);
        SetDirty();
    }

    void Transform::QuaternionRotate(const quaternion& quaternion)
    {
        m_rotation *= quaternion;
    }

    void Transform::DegreesRotate(const vec3& eulerDegrees)
    {
        RadiansRotate(glm::radians(eulerDegrees));
    }

    void Transform::RadiansRotate(const vec3& eulerRadians)
    {
        m_rotation *= glm::quat(eulerRadians);
        SetDirty();
    }

    void Transform::DegreesRotateAroundAxis(const vec3& axis, float degrees)
    {
        RadiansRotateAroundAxis(axis, glm::radians(degrees));
    }

    void Transform::RadiansRotateAroundAxis(const vec3& axis, float radians)
    {
        m_rotation = glm::normalize(glm::angleAxis(radians, glm::normalize(axis)) * m_rotation);
        SetDirty();
    }
    
    void Transform::LookAt(const vec3& target, const vec3& up)
    {
        vec3 forward = glm::normalize(target - m_position);
        vec3 right = glm::normalize(glm::cross(up, forward));
        vec3 correctedUp = glm::cross(forward, right);

        glm::mat3 rotMat(right, correctedUp, forward);
        m_rotation = glm::quat_cast(rotMat);
        SetDirty();
    }

    void Transform::DegreesRotateLocal(const vec3& degrees)
    {
        RadiansRotateLocal(glm::radians(degrees));
    }

    void Transform::RadiansRotateLocal(const vec3& eulerRadians)
    {
        RecalculateCache();

        if (eulerRadians.y != 0.0f)
            m_rotation *= glm::normalize(glm::angleAxis(eulerRadians.y, m_right));
        if (eulerRadians.x != 0.0f)
            m_rotation *= glm::normalize(glm::angleAxis(eulerRadians.x, m_up));
        if (eulerRadians.z != 0.0f)
            m_rotation *= glm::normalize(glm::angleAxis(eulerRadians.z, m_forward));

        SetDirty();
    }
#pragma endregion

#pragma region Scale
    const vec3& Transform::GetScale() const
    {
        return m_scale;
    }

    void Transform::SetScale(const vec3& scale)
    {
        m_scale = scale;
        SetDirty();
    }

    void Transform::Scale(const vec3& scale)
    {
        m_scale += scale;
        SetDirty();
    }
#pragma endregion

#pragma region Vectors
    const vec3& Transform::Right() const
    {
        RecalculateCache();
        return m_right;
    }

    const vec3& Transform::Up() const
    {
        RecalculateCache();
        return m_up;
    }

    const vec3& Transform::Forward() const
    {
        RecalculateCache();
        return m_forward;
    }
#pragma endregion

    void Transform::RecalculateCache() const
    {
        if (!m_dirty)
            return;

        m_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);

        m_right = glm::normalize(m_rotation * glm::vec3(1, 0, 0));
        m_up = glm::normalize(m_rotation * glm::vec3(0, 1, 0));
        m_forward = glm::normalize(m_rotation * glm::vec3(0, 0, 1));

        m_dirty = false;
    }

    void Transform::SetDirty() const
    {
        m_dirty = true;
        if (OnTransformDirty)
            OnTransformDirty();
    }
}