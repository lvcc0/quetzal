#include "model.h"
#include <glm/gtx/string_cast.hpp>

Model::Model(std::string const& path)
{
    loadModel(path); // read obj file
    setupModel();    // setup VAO, VBO, EBO
}

void Model::Draw(Shader& shader)
{
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;

    // Assign all texture fragment shader uniforms
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = m_textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    // Draw via indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Model::setupModel()
{
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Element Array Buffer
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

void Model::loadModel(std::string const& path)
{
    m_directory = path.substr(0, path.find_last_of('/'));

    std::ifstream file(path);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    std::vector<std::string> triplets; // vector for face vertices ("v/vt/vn")
    glm::vec3 vector3;                 // vec3 for v, vn
    glm::vec2 vector2;                 // vec2 for vt

    std::string line;

    while (std::getline(file, line))
    {
        std::string linetype = line.substr(0, line.find(" "));

        if (linetype ==  "#") // comment
            continue;

        if (linetype == "mtllib") // material file
        {
            std::istringstream v(line.substr(line.find(" ") + 1));
            loadMaterial(v.str());
        }

        if (linetype == "o") // object (mesh)
        {
            // think something
        }

        if (linetype == "v") // vertex
        {
            std::istringstream v(line.substr(line.find(" ")));

            v >> vector3.x;
            v >> vector3.y;
            v >> vector3.z;

            positions.push_back(vector3);
        }

        if (linetype == "vt") // texture
        {
            std::istringstream v(line.substr(line.find(" ")));

            v >> vector2.x;
            v >> vector2.y;

            texCoords.push_back(vector2);
        }

        if (linetype == "vn") // normal
        {
            std::istringstream v(line.substr(line.find(" ")));

            v >> vector3.x;
            v >> vector3.y;
            v >> vector3.z;

            normals.push_back(vector3);
        }

        if (linetype == "f") // face
        {
            /**
             *  v - "v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
             *  points - {"v1/vt1/vn1", "v2/vt2/vn2", "v3/vt3/vn3"}
             *  token - temporal string to get point's v, vt, vn
             *  tokens - temporal vector to store point's v, vt, vn and push them to vertex 
             */

            std::istringstream v(line.substr(line.find(" ")));

            std::string points[3];
            std::string token;
            std::vector<int> tokens;

            v >> points[0];
            v >> points[1];
            v >> points[2];

            for (unsigned int i = 0; i < 3; i++)
            {
                std::stringstream point(points[i]);

                // Check for already existing triplets (v/vt/vn) and push index of the first mention of them to indices
                triplets.push_back(point.str());
                m_indices.push_back(std::find(triplets.begin(), triplets.end(), point.str()) - triplets.begin());

                // "v/vt/vn" -> {v, vt, vn}
                while (getline(point, token, '/'))
                    tokens.push_back(std::stoi(token));

                m_vertices.push_back(
                    Vertex(positions[--tokens[0]], 
                           texCoords[--tokens[1]], 
                           normals[--tokens[2]])
                );

                tokens.clear();
            }
        }
    }
}

void Model::loadMaterial(std::string const& path)
{
    std::ifstream file(m_directory + '/' + path);
    std::string line;

    while (std::getline(file, line))
    {
        std::string linetype = line.substr(0, line.find(" "));

        if (linetype == "map_Kd")
        {
            std::string str = line.substr(line.find(" ") + 1);
            bool skip = false;
            
            for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
            {
                if (std::strcmp(m_textures_loaded[j].path.data(), str.c_str()) == 0)
                {
                    m_textures.push_back(m_textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture texture;
                texture.id = TextureFromFile(str.c_str(), m_directory);
                texture.type = "texture_diffusal";
                texture.path = str.c_str();

                m_textures.push_back(texture);
                m_textures_loaded.push_back(texture);
            }
        }
    }
}

Billboard::Billboard(glm::vec3 pos, glm::vec2 size, std::string const& texture_path)
{
    m_pos = pos;
    m_size = size;
    m_transform = glm::mat4(1.0f);

    m_vertices =
    {
        Vertex(glm::vec3(-m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(-m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3( m_size[0] / 2.0f,  m_size[1] / 2.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    };

    for (Vertex vertex : m_vertices)
    {
        vertex.Position += m_pos;
    }

    m_indices =
    {
        0, 1, 2,
        2, 3, 0
    };

    Texture texture;
    texture.id = TextureFromFile(texture_path.c_str(), "");
    texture.type = "texture_diffuse";
    texture.path = texture_path.c_str();

    m_texture = texture;

    setupBillboard();
}

void Billboard::setupBillboard()
{
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Array Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // Element Array Buffer
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

void Billboard::Draw(Shader& shader, glm::vec3 camPos)
{
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("material.texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, m_texture.id);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = !directory.empty() ? directory + '/' + path : path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, numComponents;
    unsigned char* bytes = stbi_load(filename.c_str(), &width, &height, &numComponents, 0);

    if (bytes)
    {
        GLenum format;

        if (numComponents == 1)
            format = GL_RED;
        else if (numComponents == 3)
            format = GL_RGB;
        else if (numComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at: " << path << std::endl;

    stbi_image_free(bytes);
    return textureID;
}