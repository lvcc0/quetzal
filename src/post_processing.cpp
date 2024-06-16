#include "post_processing.h"
#include "post_processing.h"
#include "post_processing.h"

PostProcessing::PostProcessing(std::map<const std::string, std::shared_ptr<Shader>>& shaderMap, GLfloat width, GLfloat height) :
    m_ShaderMap(shaderMap)
{
    this->setupBuffers(VAO, VBO);
    this->setupFramebuffer(m_Framebuffer, m_TextureFramebuffer, m_RenderFramebuffer, width, height);
}

PostProcessing::~PostProcessing()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &m_Framebuffer);
    glDeleteBuffers(1, &m_TextureFramebuffer);
    glDeleteBuffers(1, &m_RenderFramebuffer);

    std::map<std::string, std::shared_ptr<Shader>>::iterator iter = m_ShaderMap.begin();
    while (iter != m_ShaderMap.end())
    {
        iter->second->deleteShader();
        iter++;
    }
}

void PostProcessing::activate(const std::shared_ptr<Shader>& screen_shader) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test
    glClear(GL_COLOR_BUFFER_BIT); // clear all relevant buffers

    screen_shader->activateShader();
    screen_shader->setInt("screenTexture", 0);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, m_TextureFramebuffer);    // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::deactivate() const
{
    // Bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    // Specify the color of the background
    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);
    // Clear the back buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Enable depth testing since it's disabled when drawing the framebuffer rectangle
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::recreate(GLuint width, GLuint height) const
{
    // Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    // Color attachment texture
    glBindTexture(GL_TEXTURE_2D, m_TextureFramebuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureFramebuffer, 0);

    // Renderbuffer (for depth and stencil attachments)
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderFramebuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderFramebuffer);

    // Check if the framebuffer we created is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER RECREATION WASN'T COMPLETED::ID " << m_Framebuffer << std::endl;

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
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    // Vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void PostProcessing::setupFramebuffer(GLuint& frameBuffer, GLuint& textureFrameBuffer, GLuint& renderFrameBuffer, GLfloat width, GLfloat height)
{
    // Framebuffer
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Color attachment texture
    glGenTextures(1, &textureFrameBuffer);
    glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFrameBuffer, 0);

    // Renderbuffer (for depth and stencil attachments)
    glGenRenderbuffers(1, &renderFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderFrameBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderFrameBuffer);

    // Check if the framebuffer we created is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER WASN'T COMPLETED::ID " << frameBuffer << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}