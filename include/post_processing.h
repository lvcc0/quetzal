#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <shader.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class PostProcessing
{
public:
    // Map of shaders
    std::map<const std::string, std::shared_ptr<Shader>> m_ShaderMap;

    // Constructor
    PostProcessing(std::map<const std::string, std::shared_ptr<Shader>>& shaderMap, GLfloat width, GLfloat height);

    // Destructor
    ~PostProcessing();

    // Postprocessing funcs
    void activate(const std::shared_ptr<Shader>& screen_shader) const;

    // Deactivating postprocessing (must be used BEFORE any postprocessing funcs and objects drawing)
    void deactivate() const;

    // Recreate framebuffer's attachments
    void recreate(GLuint width, GLuint height) const;

private:
    // Buffers
    GLuint VAO, VBO;
    GLuint m_Framebuffer, m_TextureFramebuffer, m_RenderFramebuffer;

    void setupBuffers(GLuint& VAO, GLuint& VBO); // setup VAO, VBO
    void setupFramebuffer(GLuint& frameBuffer, GLuint& textureFrameBuffer, GLuint& renderFrameBuffer, GLfloat width, GLfloat height); // setup framebuffer and all the corresponding ones

    std::vector<float> quadVertices = // a quad that fills the entire screen
    {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,

        -1.0f,  1.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f
    };
};