#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
private:
    glm::vec3 m_Position = glm::vec3(0, 0, 0);
    glm::quat m_Rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 m_Scale = glm::vec3(1, 1, 1);

    bool m_Dirty = true;
    glm::mat4x4 m_ModelMatrix{};

private:
    void recomputeModelMatrix();

public:
    Transform();
    Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
    Transform(const Transform& rhs);
    Transform(Transform&& rhs) noexcept;

    bool operator==(const Transform& rhs) const;
    bool operator!=(const Transform& rhs) const;
    Transform& operator=(const Transform& rhs);
    Transform& operator=(Transform&& rhs) noexcept;

    void reset(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

    const glm::vec3& position() const
    {
        return m_Position;
    }

    void setPosition(const glm::vec3& pos);

    const glm::quat& rotation() const
    {
        return m_Rotation;
    }

    void setRotation(const glm::quat& rotation);

    const glm::vec3& scale() const
    {
        return m_Scale;
    }

    void setScale(const glm::vec3& scale);

    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    const glm::mat4x4& modelMatrix();

    void localTranslate(const glm::vec3& offset);
    void worldTranslate(const glm::vec3& offset);

    void localRotate(float angle, const glm::vec3& axis);
    void worldRotate(float angle, const glm::vec3& axis);

    void lookAt(const glm::vec3& target);
    void lookAt(const glm::vec3& target, const glm::vec3& worldUp);

    // statics
private:
    static glm::vec3 s_WorldForward;
    static glm::vec3 s_WorldRight;
    static glm::vec3 s_WorldUp;

public:
    static void modEulerAngle(float& angle)
    {
        angle = fmod(angle, 360.0f);
    }

    static void modEulerAngles(glm::vec3& angles)
    {
        modEulerAngle(angles.x);
        modEulerAngle(angles.y);
        modEulerAngle(angles.z);
    }

    static const glm::vec3& worldForward()
    {
        return s_WorldForward;
    }

    static const glm::vec3& worldRight()
    {
        return s_WorldRight;
    }

    static const glm::vec3& worldUp()
    {
        return s_WorldUp;
    }
};
