#pragma once

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/main/node.h"

namespace qtzl
{
    // Base class for every 3D node
    class Node3D : public Node
    {
    public:
        Node3D(const std::string& name);
        virtual ~Node3D() = default;

        void setScale(const glm::vec3& scale);

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& radians);
        void setRotationDegrees(const glm::vec3& degrees);
        
        void setGlobalPosition(const glm::vec3& position);
        void setGlobalRotation(const glm::vec3& radians);
        void setGlobalRotationDegrees(const glm::vec3& degrees);
        
        void setVisible(bool visible);

        glm::vec3 getScale() const;

        glm::vec3 getPosition() const;
        glm::vec3 getRotation() const;
        glm::vec3 getRotationDegrees() const;
        
        glm::vec3 getGlobalPosition() const;
        glm::vec3 getGlobalRotation() const;
        glm::vec3 getGlobalRotationDegrees() const;

        bool isVisible() const;

        void show();
        void hide();

    protected:
        bool m_Visible = true;

        glm::vec3 m_Scale = glm::vec3(1.0f);

        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::vec3 m_GlobalPosition = glm::vec3(0.0f);

        glm::vec3 m_Rotation = glm::vec3(0.0f);       // in radians
        glm::vec3 m_GlobalRotation = glm::vec3(0.0f); // in radians
    };
}