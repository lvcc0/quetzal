#include "post_processing.h"

PostProcessing::PostProcessing(std::map<const std::string, std::shared_ptr<Shader>>& shaderMap, GLfloat width, GLfloat height)
    : m_ShaderMap(shaderMap)
{
    this->setupBuffers(VAO, VBO);
    this->setupFramebuffer(m_FirstFBO, m_FirstColorAttachment, m_RBO, width, height);
    this->setupFramebuffer(m_SecondFBO, m_SecondColorAttachment, m_RBO, width, height);
}

PostProcessing::~PostProcessing()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteBuffers(1, &m_FirstFBO);
    glDeleteBuffers(1, &m_SecondFBO);

    glDeleteBuffers(1, &m_FirstColorAttachment);
    glDeleteBuffers(1, &m_SecondColorAttachment);

    glDeleteBuffers(1, &m_RBO);

    std::map<std::string, std::shared_ptr<Shader>>::iterator iter = m_ShaderMap.begin();
    while (iter != m_ShaderMap.end())
    {
        iter->second->deleteShader();
        iter++;
    }
}

void PostProcessing::activate() const
{
    // So called "ping-pong shading" ahead!

    auto i = 0;
    for (auto item: m_ActiveShaders)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, (i % 2) ? m_FirstFBO : m_SecondFBO); // if i is odd, then we bind first fbo else we bind the second one
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        item->activateShader();
        item->setInt("screenTexture", 0);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, (i % 2) ? m_SecondColorAttachment : m_FirstColorAttachment); // if i is odd, then we bind second color attachment else we bind the first one
        glDrawArrays(GL_TRIANGLES, 0, 6);

        i++;
    }
    
    // Don't forget to attach the default fbo to actually draw stuff on the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, (i % 2) ? m_FirstColorAttachment : m_SecondColorAttachment); // set the next color attachment to draw all shader effects
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::deactivate() const
{
    // Bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FirstFBO);
    // Specify the color of the background
    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);
    // Clear the back buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Enable depth testing since it's disabled when drawing the framebuffer rectangle
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::recreate(GLuint width, GLuint height) const
{
    // Renderbuffer (for depth and stencil attachments)
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    
    // First framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FirstFBO);

    // First color attachment texture
    glBindTexture(GL_TEXTURE_2D, m_FirstColorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FirstColorAttachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Second framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_SecondFBO);

    // Second color attachment texture
    glBindTexture(GL_TEXTURE_2D, m_SecondColorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SecondColorAttachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::setScreenShader(const std::string& name, bool enabled)
{
    std::vector<std::string>& currentScreenShaderNames = getScreenShaders();

    if (enabled)
        this->m_ActiveShaders.push_back(this->m_ShaderMap.at(name));
    else if (std::find(currentScreenShaderNames.begin(), currentScreenShaderNames.end(), name) != currentScreenShaderNames.end())
        this->m_ActiveShaders.erase(std::find(m_ActiveShaders.begin(), m_ActiveShaders.end(), this->m_ShaderMap.at(name)));
} 

std::vector<std::string> PostProcessing::getScreenShaders()
{
    std::vector<std::string> result;

    for (const auto& entry : this->m_ShaderMap)
        if (std::find(this->m_ActiveShaders.begin(), this->m_ActiveShaders.end(), entry.second) != this->m_ActiveShaders.end())
            result.push_back(entry.first);

    return result;
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