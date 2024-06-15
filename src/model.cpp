#include "model.h"

Model::Model(std::vector<Vertex>& vertices,
             std::vector<unsigned int>& indices,
             std::vector<std::shared_ptr<Texture>>& textures) :
    m_textures(textures)
{
    m_vertices.swap(vertices);
    m_indices.swap(indices);

    setupModel(); // setup VAO, VBO, EBO
}

Model::Model(const Model& obj) :
    m_indices(obj.m_indices),
    m_textures(obj.m_textures),
    m_model_matrix(obj.m_model_matrix),
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

void Model::Draw(std::shared_ptr<Shader>& shader)
{
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;

    // Assign all texture fragment shader uniforms
    for (auto i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = m_textures[i]->m_type;
        
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);

        shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->ID);
    }

    glActiveTexture(GL_TEXTURE0);

    // Convert local coordinates to world coordinates
    shader->setMat4("model", m_model_matrix);
    shader->setMat4("inversed", glm::inverse(m_model_matrix));

    m_model_matrix = glm::mat4(1.0f);

    // Draw via indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Model::translate(glm::vec3 vector)
{
    m_model_matrix = glm::translate(m_model_matrix, vector);
}

void Model::scale(glm::vec3 vector)
{
    m_model_matrix = glm::scale(m_model_matrix, vector);
}

void Model::rotate(float degrees, glm::vec3 vector)
{
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(degrees), vector);
}

void Model::setupModel()
{
    // Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Element Array Buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

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