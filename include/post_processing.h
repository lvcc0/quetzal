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
    std::vector<std::shared_ptr<Shader>> m_ActiveShaders;

    // Constructor
    PostProcessing(std::map<const std::string, std::shared_ptr<Shader>>& shaderMap, GLfloat width, GLfloat height);

    // Destructor
    ~PostProcessing();

    void deactivate() const;                                                        // deactivating postprocessing (must be used BEFORE any postprocessing funcs and objects drawing)
    void activate() const; // postprocessing funcs
    void recreate(GLuint width, GLuint height) const;                               // recreate framebuffer's attachments

    void setScreenShader(const std::string& name, bool enabled); // set postprocessing shader on or off

    std::vector<std::string> getScreenShaders(); // get vector of active screen shader names

private:
    // Buffers
    GLuint VAO, VBO;

    GLuint m_RBO;                                           // only one renderbuffer object for both fbos
    GLuint m_FirstFBO, m_SecondFBO;                         // 2 framebuffer objects for ping-pong shading
    GLuint m_FirstColorAttachment, m_SecondColorAttachment; // 2 color (texture) attachments for 2 fbos

    void setupBuffers(GLuint& VAO, GLuint& VBO);                                                             // setup VAO, VBO
    void setupFramebuffer(GLuint& FBO, GLuint& colorAttachment, GLuint& RBO, GLfloat width, GLfloat height); // setup framebuffer and all the corresponding ones

    std::vector<float> m_QuadVertices = // a quad that fills the entire screen
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