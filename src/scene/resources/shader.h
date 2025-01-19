#pragma once

// thirdparty
#include <glad/glad.h>

#include "scene/main/resource.h"

namespace qtzl
{
    // Shader resource
    class Shader : public Resource
    {
    public:
        Shader(const std::string& name, const std::string& path, const std::string& code, GLenum shader_type);        
        virtual ~Shader() = default;

        GLuint getID() const;

    private:
        GLuint ID;
    };
}