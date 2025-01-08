#include "core/rendering/post_processing.h"

PostProcessing::PostProcessing(GLfloat viewport_width, GLfloat viewport_height)
{
    for (const auto& shaderProgram : ResourceManager::getPPShaderPrograms())
    {
        this->m_ShaderPrograms.emplace(shaderProgram, false);
    }

    this->setupBuffers(this->VAO, this->VBO);
    this->setupFramebuffer(this->m_FirstFBO, this->m_FirstColorAttachment, this->m_RBO, viewport_width, viewport_height);
    this->setupFramebuffer(this->m_SecondFBO, this->m_SecondColorAttachment, this->m_RBO, viewport_width, viewport_height);
}

PostProcessing::~PostProcessing()
{
    glDeleteBuffers(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);

    glDeleteBuffers(1, &this->m_FirstFBO);
    glDeleteBuffers(1, &this->m_SecondFBO);

    glDeleteBuffers(1, &this->m_FirstColorAttachment);
    glDeleteBuffers(1, &this->m_SecondColorAttachment);

    glDeleteBuffers(1, &this->m_RBO);

    for (const auto& entry : this->m_ShaderPrograms)
        entry.first->deleteProgram();
}

void PostProcessing::setPPShaderEnabled(const std::string& name, bool enabled)
{
    for (auto& entry : m_ShaderPrograms)
    {
        if (entry.first->getName() == name)
            entry.second = enabled;
    }
}

std::vector<std::string> PostProcessing::getEnabledPPShaders() const
{
    std::vector<std::string> result;

    for (const auto& entry : m_ShaderPrograms)
    {
        if (entry.second)
            result.push_back(entry.first->getName());
    }

    return result;
}

bool PostProcessing::isActive() const
{
    for (const auto& entry : m_ShaderPrograms)
    {
        if (entry.second)
            return true
    }

    return false;
}

void PostProcessing::enablePPShader(const std::string& name)
{
    this->setPPShaderEnabled(name, true);
}

void PostProcessing::disablePPShader(const std::string& name)
{
    this->setPPShaderEnabled(name, false);
}

void PostProcessing::deactivate() const
{
    // Bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_FirstFBO);
    // Specify the color of the background
    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);
    // Clear the back buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Enable depth testing since it's disabled when drawing the framebuffer rectangle
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::activate() const
{
    // So called "ping-pong shading" ahead!

    unsigned int i = 0;
    for (const auto& entry : m_ShaderPrograms)
    {
        if (!entry.second)
            continue;

        // If i is odd, then we bind first fbo else we bind the second one
        glBindFramebuffer(GL_FRAMEBUFFER, (i % 2) ? this->m_FirstFBO : this->m_SecondFBO);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        entry.first->activateProgram();
        entry.first->setInt("screenTexture", 0);

        // If i is odd, then we bind second color attachment else we bind the first one
        glBindVertexArray(this->VAO);
        glBindTexture(GL_TEXTURE_2D, (i % 2) ? this->m_SecondColorAttachment : this->m_FirstColorAttachment);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        i++;
    }
    
    // Don't forget to attach the default fbo to actually draw stuff on the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the next color attachment to draw all shader effects
    glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_2D, (i % 2) ? this->m_FirstColorAttachment : this->m_SecondColorAttachment);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::recreate(GLuint width, GLuint height) const
{
    // Renderbuffer (for depth and stencil attachments)
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    
    // First framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_FirstFBO);

    // First color attachment texture
    glBindTexture(GL_TEXTURE_2D, this->m_FirstColorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_FirstColorAttachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Second framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_SecondFBO);

    // Second color attachment texture
    glBindTexture(GL_TEXTURE_2D, this->m_SecondColorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_SecondColorAttachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::setupBuffers(GLuint& VAO, GLuint& VBO)
{
    // Vertxe Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_QuadVertices.size() * sizeof(float), &m_QuadVertices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    // Vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void PostProcessing::setupFramebuffer(GLuint& FBO, GLuint& colorAttachment, GLuint& RBO, GLfloat width, GLfloat height)
{
    // Framebuffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Color attachment texture
    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

    // Renderbuffer (for depth and stencil attachments)
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Check if the framebuffer we created is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER WASN'T COMPLETED::ID " << FBO << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}