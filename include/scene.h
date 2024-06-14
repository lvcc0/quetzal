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

enum class ObjectType { SHADER, TEXTURE, MODEL, CYL_BILLBOARD, SPH_BILLBOARD };

class Scene
{
public:
    // Camera (TODO: make it just a camera object, not a pointer perhaps)
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<PostProcessing> m_PostProcessing; // basically creating a quad that fills the whole screen allowing some funky shader shenanigans

    // Constructor
    Scene(Camera& camera);

    // Destructor
    ~Scene();

    // Gets called every frame in the engine class
    void update();

    void setShader(const std::string& name); // set shader to draw stuff with
    void setScreenShader(const std::string& name); // set postprocessing shader

    // Some stuff to add to the scene
    std::shared_ptr<Shader> addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
    std::shared_ptr<Texture> addTexture(std::string name, std::string type, const std::string& texture_rel_path);
    std::shared_ptr<Model> addModel(std::string name, const std::string& model_rel_path);
    std::shared_ptr<CylindricalBillboard> addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);
    std::shared_ptr<SphericalBillboard> addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);

    // Some stuff to copy in the scene
    std::shared_ptr<Model> copyModel(std::string name, const std::shared_ptr<Model> const model);
    std::shared_ptr<CylindricalBillboard> copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> const cyl_billboard);
    std::shared_ptr<SphericalBillboard> copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> const sph_billboard);

    // Add lights to the scene
    std::shared_ptr<DirLight> addDirLight(DirLight& dir_light);
    std::shared_ptr<PointLight> addPointLight(PointLight& point_light);
    std::shared_ptr<SpotLight> addSpotLight(SpotLight& spot_light);
    
    // Print objects in maps
    void getObjectsInMaps(ObjectType objectType);

private:
    // Vectors with scene lights
    std::vector<std::shared_ptr<DirLight>> m_DirLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>> m_SpotLights;
    
    std::shared_ptr<Shader> m_CurrentShader; // a shader to draw stuff with
    std::shared_ptr<Shader> m_CurrentScreenShader; // a postprocessing shader

    // --- Maps of loaded objects --- //
    typedef std::map<const std::string, std::shared_ptr<Shader>> ShaderMap;
    ShaderMap shaderMap;

    typedef std::map<const std::string, std::shared_ptr<Model>> ModelMap;
    ModelMap modelMap;

    typedef std::map<const std::string, std::shared_ptr<Texture>> TextureMap;
    TextureMap textureMap;

    typedef std::map<const std::string, std::shared_ptr<CylindricalBillboard>> CylBillboardMap;
    CylBillboardMap cylBillboardMap;

    typedef std::map<const std::string, std::shared_ptr<SphericalBillboard>> SphBillboardMap;
    SphBillboardMap sphBillboardMap;
    // --- //
};