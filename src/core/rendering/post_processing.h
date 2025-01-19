#pragma once

// std
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "static/resource_manager.h"
#include "core/rendering/shader_program.h"

class PostProcessing
{
public:
    PostProcessing(GLfloat viewport_width, GLfloat viewport_height);
    ~PostProcessing();

    void setPPShaderEnabled(const std::string& name, bool enabled);

    std::vector<std::string>                    getEnabledPPShaders() const;
    std::vector<std::string>                    getPPShaderNames() const;
    std::vector<std::shared_ptr<ShaderProgram>> getPPShaders() const;

    // Returns true if at least one shader program is enabled
    bool isActive() const;

    void enablePPShader(const std::string& name);
    void disablePPShader(const std::string& name);

    // deactivating postprocessing (must be used BEFORE any postprocessing funcs and objects rendering)
    void deactivate() const;
    
    // postprocessing funcs
    void activate() const;
    
    // recreate framebuffer's attachments
    void recreate(GLuint width, GLuint height) const;

private:
    std::map<std::shared_ptr<ShaderProgram>, bool> m_ShaderPrograms; // *ShaderProgram, is_enabled

    GLuint VAO, VBO;
    GLuint m_RBO;                                           // only one renderbuffer object for both fbos
    GLuint m_FirstFBO, m_SecondFBO;                         // 2 framebuffer objects for ping-pong shading
    GLuint m_FirstColorAttachment, m_SecondColorAttachment; // 2 color (texture) attachments for 2 fbos

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
    
    // setup VAO, VBO
    void setupBuffers(GLuint& VAO, GLuint& VBO);
    
    // setup framebuffer and all the corresponding ones
    void setupFramebuffer(GLuint& FBO, GLuint& colorAttachment, GLuint& RBO, GLfloat width, GLfloat height);
};