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
      m_ModelMatrix(obj.m_ModelMatrix),
      VAO(obj.VAO)
{ /* empty */ }

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

void Model::draw(std::shared_ptr<Shader>& shader)
{
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;

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

        shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID);
    }

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_ModelMatrix);
    shader->setMat4("inversed", glm::inverse(m_ModelMatrix));

    // Draw via indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
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