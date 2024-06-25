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

#include "resource_manager.h"
#include "post_processing.h"
#include "camera.h"
#include "lights.h"

#include "shader.h"
#include "model.h"
#include "texture.h"
#include "billboards.h"
#include "physics.h"

enum class ObjectType { SHADER, TEXTURE, MODEL, CYL_BILLBOARD, SPH_BILLBOARD };

class Scene
{
public:
    std::shared_ptr<Camera>                 m_Camera;         // TODO: make it just a camera object, not a pointer perhaps)
    std::shared_ptr<PostProcessing>         m_PostProcessing; // basically creating a quad that fills the whole screen allowing some funky shader shenanigans
    std::vector<std::shared_ptr<RigidBody>> m_RigidBodies;

    bool m_IsPostProcessing = false; // postprocessing bool

    // Constructor
    Scene(Camera& camera);

    // Destructor
    ~Scene();

    // Gets called every frame in the engine class
    void update();

    // Enable post processing
    void enablePostProcessing();

    void setShader(const std::string& name);                     // set shader to draw stuff with (only one for now (ig it'll always be only one))
    void setScreenShader(const std::string& name, bool enabled); // set postprocessing shader on or off

    const std::string        getShader();        // get active shader name
    std::vector<std::string> getScreenShaders(); // get vector of active screen shader names
    
    std::map<const std::string, std::shared_ptr<Shader>>               getShaderMap();
    std::map<const std::string, std::shared_ptr<Texture>>              getTextureMap();
    std::map<const std::string, std::shared_ptr<Model>>                getModelMap();
    std::map<const std::string, std::shared_ptr<CylindricalBillboard>> getCylindricalBillboardMap();
    std::map<const std::string, std::shared_ptr<SphericalBillboard>>   getSphericalBiillboardMap();

    // Do collisions
    void doPhysicsProcessing();
    
    // Some stuff to add to the scene
    std::shared_ptr<Shader>               addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
    std::shared_ptr<Texture>              addTexture(std::string name, std::string type, const std::string& texture_rel_path);
    std::shared_ptr<Model>                addModel(std::string name, const std::string& model_rel_path);
    std::shared_ptr<RigidBody>            addRigidBody(std::string name, const std::string& model_rel_path, Collision& collision);
    std::shared_ptr<CylindricalBillboard> addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);
    std::shared_ptr<SphericalBillboard>   addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);

    // Some stuff to copy in the scene
    std::shared_ptr<Model>                copyModel(std::string name, const std::shared_ptr<Model> model);
    std::shared_ptr<CylindricalBillboard> copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> cyl_billboard);
    std::shared_ptr<SphericalBillboard>   copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> sph_billboard);

    // Some stuff to delete in the scene
    void deleteModel(std::string name, std::shared_ptr<Model>& model);
    void deleteCylBillboard(std::string name, std::shared_ptr<CylindricalBillboard>& cyl_billboard);
    void deleteSphBillboard(std::string name, std::shared_ptr<SphericalBillboard>& sph_billboard);

    // Add lights to the scene
    std::shared_ptr<DirLight>   addDirLight(DirLight& dir_light);
    std::shared_ptr<PointLight> addPointLight(PointLight& point_light);
    std::shared_ptr<SpotLight>  addSpotLight(SpotLight& spot_light);

    // Print objects in maps
    void printObjectsInMaps(ObjectType objectType);

private:
    // Vectors with scene lights
    std::vector<std::shared_ptr<DirLight>>   m_DirLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>>  m_SpotLights;

    std::shared_ptr<Shader>              m_CurrentShader;        // a shader to draw stuff with
    std::vector<std::shared_ptr<Shader>> m_CurrentScreenShaders; // vector of postprocessing shaders

    // Maps of loaded objects
    std::map<const std::string, std::shared_ptr<Shader>>               m_ShaderMap;
    std::map<const std::string, std::shared_ptr<Texture>>              m_TextureMap;
    std::map<const std::string, std::shared_ptr<Model>>                m_ModelMap;
    std::map<const std::string, std::shared_ptr<RigidBody>>            m_RigidBodyMap;
    std::map<const std::string, std::shared_ptr<CylindricalBillboard>> m_CylBillboardMap;
    std::map<const std::string, std::shared_ptr<SphericalBillboard>>   m_SphBillboardMap;
};