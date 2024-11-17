#include "model.h"

Model::Model(std::vector<Vertex>& vertices,
             std::vector<unsigned int>& indices,
             std::vector<std::shared_ptr<Texture>>& textures,
             bool is_preload, std::string name)
    :Renderable(vertices, indices), Scene_Node(name), Scene_Object(), m_Textures(textures)
{
    if (!is_preload)
        setupRender(); // setup VAO, VBO, EBO
}

Model::Model(const Model& obj)
    : Renderable(obj), Scene_Node(obj), Scene_Object(obj),
    m_Textures(obj.m_Textures)
{
    setupRender();
}

void Model::draw(const Shaders_pack& shaders) 
{
    std::shared_ptr<Shader> main_shader = shaders.MAIN_SHADER;
    std::shared_ptr<Shader> stencil_shader = shaders.STENCIL_SHADER;
    
    // Inheritor methods can be called
    this->setPosition(m_Position);
    this->setScale(m_Scale);
    this->setRotationDegrees(glm::vec3(1.0, 0.0, 0.0), m_RotationDegrees.x);
    this->setRotationDegrees(glm::vec3(0.0, 1.0, 0.0), m_RotationDegrees.y);
    this->setRotationDegrees(glm::vec3(0.0, 0.0, 1.0), m_RotationDegrees.z);

    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;

    main_shader->activateShader(); // Using default shader

    // Assign all texture fragment shader uniforms
    for (auto i = 0; i < m_Textures.size(); i++)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));

        std::string number;
        std::string name = m_Textures[i]->m_type;
        
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);

        main_shader->setInt(("material." + name + number).c_str(), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID));
    }

    GLCall(glActiveTexture(GL_TEXTURE0));

    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    // Draw via indices
    // Part working if model selected
    if (is_selected && stencil_shader != nullptr)
    {
        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        vao_ptr->bind();
        GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));

        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GLCall(glStencilMask(0x00));
        GLCall(glDisable(GL_DEPTH_TEST));

        stencil_shader->activateShader(); // Using special stencil shader
        for (auto i = 0; i < m_Textures.size(); i++)
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + i));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID));
        }

        // Convert local coordinates to world coordinates
        stencil_shader->setMat4("model", glm::scale(m_ModelMatrix, glm::vec3(m_StencilScaling, m_StencilScaling, m_StencilScaling)));
        GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
        vao_ptr->unbind();

        GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));
        GLCall(glStencilMask(0xFF));
        GLCall(glEnable(GL_DEPTH_TEST));

        main_shader->activateShader(); // Return default shader
    }
    // Part working if model not selected
    else 
    {
        vao_ptr->bind();
        GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
        vao_ptr->unbind();
    }

    m_ModelMatrix = glm::mat4(1.0f);
}

inline void Model::setPosition(const glm::vec3 pos)
{
    m_ModelMatrix = glm::translate(m_ModelMatrix, pos);
    m_Position = pos;
}

inline void Model::setScale(const glm::vec3 scale)
{
    m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
    m_Scale = scale;
}

inline void Model::setRotationDegrees(const glm::vec3 rotation, float degrees)
{
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(degrees), rotation);
    m_RotationDegrees = rotation * degrees;
}

glm::mat4 Model::getModelMatrix() const
{
    glm::mat4 return_matrix = glm::mat4(1.0f);
    return_matrix = glm::translate(return_matrix, m_Position);
    return_matrix = glm::scale(return_matrix, m_Scale);
    return_matrix = glm::rotate(return_matrix, glm::radians(m_RotationDegrees.x), glm::vec3(1.0, 0.0, 0.0));
    return_matrix = glm::rotate(return_matrix, glm::radians(m_RotationDegrees.y), glm::vec3(0.0, 1.0, 0.0));
    return_matrix = glm::rotate(return_matrix, glm::radians(m_RotationDegrees.z), glm::vec3(0.0, 0.0, 1.0));

    return return_matrix;
}

void Model::setupRender()
{
    // Vertex Array Object
    vao_ptr = std::make_unique<VAO>();

    // Vertex Buffer Object
    vbo_ptr = std::make_unique<VBO>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));

    // Element Array Buffer
    ibo_ptr = std::make_unique<IBO>(&m_Indices[0], m_Indices.size());

    VB_Vertex_Layout layout;
    layout.push<GLfloat>(3, offsetof(Vertex, Position));
    layout.push<GLfloat>(2, offsetof(Vertex, TexCoord));
    layout.push<GLfloat>(3, offsetof(Vertex, Normal));

    vao_ptr->addBuffer(*vbo_ptr, layout);
}
