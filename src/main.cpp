#include "engine.h"

int main()
{
    // Setting these verts as static part of billboard isnt working (idk why)
    std::vector<Vertex> billboard_verts = {
        Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec2(0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper right
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // upper left
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower right
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // lower left
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f))  // upper left
    };

    const unsigned int WIN_WIDTH = 1280;
    const unsigned int WIN_HEIGHT = 720;
    
    Engine& engine = Engine::Instance(WIN_WIDTH, WIN_HEIGHT);
    ResourceManager::preLoadResources();

    auto first_scene = engine.createScene("first_scene");

    first_scene->addShader("default_shader", "shaders/default.vert", "shaders/default.frag", ShaderType::MAIN);
    first_scene->addShader("stencil_shader", "shaders/stencil.vert", "shaders/stencil.frag", ShaderType::STENCIL);

    first_scene->addSphBillboard("pepeboard", glm::vec3(0.0f), glm::vec2(7.5f, 5.0f), "pepe", billboard_verts);
    first_scene->addCylBillboard("container_billboard", glm::vec3(-5.0f, -2.0f, 0.0f), glm::vec2(4.0f, 4.0f), "container", billboard_verts);

    first_scene->addDirLight(DirLight(
        true,
        "dirLight0",
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 1.0f, 1.0f)
    ));

    first_scene->addPointLight(PointLight(
        true,
        "pointLight0",
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(1.0f, 0.0f, 0.0f)
    ), billboard_verts);

    first_scene->addSpotLight(SpotLight(
        true,
        "spotLight0",
        glm::vec3(5.0f, 0.0f, -3.5f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        7.5f,
        10.0f,
        glm::vec3(0.0f, 0.0f, 1.0f)
    ), billboard_verts);

    // Main loop
    while (engine.isRunning())
    {
        engine.process();
    }
}