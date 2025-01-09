#pragma once

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/main/node.h"

// TODO: make a class for visual 3d stuff?

namespace qtzl
{
    // Base class for every 3D node
    class Node3D : public Node
    {
    public:
        Node3D(const std::string& name);
        virtual ~Node3D() = default;

        void setPosition(const glm::vec3& position);
        void setRotationDegrees(const glm::vec3& degrees);
        void setScale(const glm::vec3& scale);
        
        void setGlobalPosition(const glm::vec3& position);
        void setGlobalRotationDegrees(const glm::vec3& degrees);
        
        void setVisible(bool visible);

        glm::vec3 getPosition() const;
        glm::vec3 getRotationDegrees() const;
        glm::vec3 getScale() const;
        
        glm::vec3 getGlobalPosition() const;
        glm::vec3 getGlobalRotationDegrees() const;

        bool isVisible() const;

        void show();
        void hide();

    protected:
        bool m_Visible = true;

        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::vec3 m_GlobalPosition = glm::vec3(0.0f);

        glm::vec3 m_RotationDegrees = glm::vec3(0.0f);
        glm::vec3 m_GlobalRotationDegrees = glm::vec3(0.0f);

        glm::vec3 m_Scale = glm::vec3(1.0f);
    };
}