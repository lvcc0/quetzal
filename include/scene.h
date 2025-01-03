#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <stb_image/stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utility>
#include <algorithm>

#include "resource_manager.h"
#include "post_processing.h"
#include "camera.h"
#include "lights.h"

#include "texture.h"
#include "billboards.h"
#include "physics.h"

enum class ObjectType { SHADER, TEXTURE, RENDERABLE };

class Scene
{
public:
    std::shared_ptr<Camera>         m_Camera;         // TODO: make it just a camera object, not a pointer perhaps
    std::shared_ptr<PostProcessing> m_PostProcessing; // basically creating a quad that fills the whole screen allowing some funky shader shenanigans

    bool m_IsPostProcessing = false; // postprocessing bool
    bool m_IsPhysics = true; // physics bool

    // Matricies
    glm::mat4 m_ProjectionMatrix;

    // Constructor
    Scene(Camera&& camera);

    // Destructor
    ~Scene();

    // Gets called every frame in the engine class
    void update();

    // Do physics
    void doPhysicsProcessing();

    // Do processing
    void doProcessing();

    // Enable post processing
    void enablePostProcessing();

    // Enable physics
    void enablePhysics();
    
    //void setShader(const std::string& name, ShaderType type);                     // set shader to draw stuff with (only one for now (ig it'll always be only one)) //IMHO: There could be more than one shader
    // TODO: REDO IT
    inline Shaders_pack                                          getActiveShaders() const { return shaders_active; };
    inline std::map<const std::string, std::shared_ptr<Texture>> getTextureMap() const { return m_TextureMap; };
    inline std::vector<std::shared_ptr<Renderable>>              getRenderableVec() const { return m_RenderableVec; };
    inline std::vector<std::shared_ptr<Scene_Node>>              getSceneNodeVec() const { return m_NodeVec; };
    inline std::vector<std::shared_ptr<Scene_Object>>            getSceneObjectVec() const { return m_SceneObjectVec; };

    // Some stuff to add to the scene
    void addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path, ShaderType type);
    void addTexture(std::string name, std::string type);
    void addModel(std::string name);
    void addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);
    void addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);

    // Add lights to the scene
    void addDirLight(DirLight dir_light);
    void addPointLight(PointLight point_light, std::vector<Vertex> verts);
    void addSpotLight(SpotLight spot_light, std::vector<Vertex> verts);

private:
    // Vectors with scene lights
    std::vector<std::shared_ptr<DirLight>>   m_DirLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>>  m_SpotLights;

    // TODO: redo it to better array
    Shaders_pack shaders_active;

    // Maps of loaded objects
    std::map<const std::string, std::shared_ptr<Texture>>   m_TextureMap;
    std::map<const std::string, std::shared_ptr<Collision>> m_CollisionMap;

    // Vectors
    std::vector<std::shared_ptr<Renderable>>   m_RenderableVec;
    std::vector<std::shared_ptr<Scene_Node>>   m_NodeVec;
    std::vector<std::shared_ptr<Scene_Object>> m_SceneObjectVec;
};