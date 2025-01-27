#pragma once

// thirdparty
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/main/node.h"

// TODO: make children visibility depend on parent's visibility
// TODO: rotating children
// TODO: fix children moving after parents
// TODO: other setters

namespace qtzl
{
    // Base class for every 3D node
    class Node3D : public Node
    {
    public:
        Node3D(const std::string& name);
        virtual ~Node3D() = default;

        virtual void set(const std::string& property_name, const glm::vec3& value) override;

        virtual void setScale(const glm::vec3& scale);

        virtual void setPosition(const glm::vec3& position);
        virtual void setRotation(const glm::vec3& radians);
        virtual void setRotationDegrees(const glm::vec3& degrees);
        
        virtual void setGlobalPosition(const glm::vec3& position);
        virtual void setGlobalRotation(const glm::vec3& radians);
        virtual void setGlobalRotationDegrees(const glm::vec3& degrees);
        
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
    };
}