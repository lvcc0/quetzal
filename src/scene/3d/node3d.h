#pragma once

#include "scene/main/node.h"

// TODO: make children visibility depend on parent's visibility
// TODO: rotating children
// TODO: fix children moving after parents
// TODO: other setters

namespace qtzl
{
    enum INTERACTION{G_POS, G_ROT_DEGR, G_ROT, POS, ROT_DEGR, ROT, SCALE, TARGET};
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

        void accept(NodeVisitor& visitor) override;

        bool m_IsVisible;
        glm::vec3 m_GlobalPosition;
        glm::vec3 m_GlobalRotation; // In degrees
        glm::vec3 m_Position;
        glm::vec3 m_Rotation; // In degrees
        glm::vec3 m_Scale;
    };
}