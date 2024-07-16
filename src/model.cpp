#include "model.h"

Model::Model(std::vector<Vertex>& vertices,
             std::vector<unsigned int>& indices,
             std::vector<std::shared_ptr<Texture>>& textures)
    : m_Textures(textures)
{
    m_Vertices.swap(vertices);
    m_Indices.swap(indices);

    setupModel(); // setup VAO, VBO, EBO
}

Model::Model(const Model& obj)
    : m_Indices(obj.m_Indices),
      m_Textures(obj.m_Textures),
      m_Vertices(obj.m_Vertices),
      m_ModelMatrix(obj.m_ModelMatrix)
{ 
    setupModel();
}

Model::~Model()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);
    
    std::cout << "model deleted with VAO::" << VAO << std::endl;

    VBO = 0;
    EBO = 0;
    VAO = 0;
}

void Model::draw(std::shared_ptr<Shader>& main_shader, std::shared_ptr<Shader>& stencil_shader)
{
    #ifdef DEBUG
    if (main_shader != nullptr)
        std::cerr << "DEBUG::MODEL::WITH VAO " << this->VAO << " ::MAIN_SHADER STATE::SET" << std::endl;
    else
        std::cerr << "DEBUG::MODEL::WITH VAO " << this->VAO << " ::MAIN_SHADER STATE::MISSING" << std::endl;

    if (stencil_shader != nullptr)
        std::cerr << "DEBUG::MODEL::WITH VAO " << this->VAO << " ::STENCIL_SHADER STATE::SET" << std::endl;
    else
        std::cerr << "DEBUG::MODEL::WITH VAO " << this->VAO << " ::STENCIL_SHADER STATE::MISSING" << std::endl;
    #endif
    
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;

    main_shader->activateShader(); // Using default shader

    // Assign all texture fragment shader uniforms
    for (auto i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = m_Textures[i]->m_type;
        
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);

        main_shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID);
    }

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    main_shader->setMat4("model", m_ModelMatrix);
    main_shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    // Draw via indices
    // Part working if model selected
    if (is_selected && stencil_shader != nullptr)
    {
        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        glStencilMask(0xFF);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);   
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        stencil_shader->activateShader(); // Using special stencil shader
        for (auto i = 0; i < m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID);
        }

        // Convert local coordinates to world coordinates
        stencil_shader->setMat4("model", glm::scale(m_ModelMatrix, glm::vec3(m_StencilScaling, m_StencilScaling, m_StencilScaling)));
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        main_shader->activateShader(); // Return default shader
    }
    // Part working if model not selected
    else 
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Model::translate(glm::vec3 vector)
{
    m_ModelMatrix = glm::translate(m_ModelMatrix, vector);
}

void Model::scale(glm::vec3 vector)
{
    m_ModelMatrix = glm::scale(m_ModelMatrix, vector);
}

void Model::rotate(float degrees, glm::vec3 vector)
{
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(degrees), vector);
}

void Model::setupModel()
{
    // Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

    // Element Array Buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    // Vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}
