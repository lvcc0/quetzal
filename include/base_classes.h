#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <concepts>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "renderer.h"
#include "buffers_objects.h"
#include "vertex_array.h"

// TODO: put them in separate files
// TODO: reorganize the file structure

namespace qtzl
{

    class Node
    {
    public:
        Node();
        virtual ~Node();

        inline std::string                                  getName() const;
        inline std::shared_ptr<Node>                        getParent() const;
        inline std::map<std::string, std::shared_ptr<Node>> getChildren() const;

        inline void setName(const std::string& name);
        inline void setParent(const std::shared_ptr<Node>& parent);

        void addChild(std::shared_ptr<Node> child);
        void removeChild(std::shared_ptr<Node> child);

        void show();
        void hide();

    protected:
        std::string m_Name;
        std::weak_ptr<Node> m_Parent;

        inline static std::map<std::string, std::weak_ptr<Node>> m_Children;
    };

    class Node3D : public Node
    {
    public:
        Node3D();
        virtual ~Node3D();

        inline glm::vec3 getPosition();
        inline glm::vec3 getScale();
        inline glm::vec3 getRotationDegrees();

        inline void setPosition();
        inline void setScale();
        inline void setRotationDegrees();

    protected:
        glm::vec3 m_LocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_GlobalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 m_LocalScale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 m_GlobalScale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 m_LocalRotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_GlobalRotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f);
    };

} // namespace qtzl