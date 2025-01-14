#pragma once

// thirdparty
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "scene/main/resource.h"

namespace qtzl
{
    // Texture resource
    class Texture : public Resource
    {
    public:
        Texture(const std::string& name, const std::string& path, const std::string& type);
        virtual ~Texture() = default;

        GLuint getID() const;
        std::string getType() const;

    private:
        GLuint ID;
        std::string m_Type;
    };
}