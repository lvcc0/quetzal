#include "core/engine.h"

int main()
{
    Engine& engine = Engine::instance(1280, 720);
    ResourceManager::preloadResources();

    auto first_scene = engine.createScene("first_scene");

    Renderer::setCurrentShaderProgram(ResourceManager::createShaderProgram("shaders/default.vert", "shaders/default.frag"));
    Renderer::setCurrentShaderProgram(ResourceManager::createShaderProgram("shaders/skybox.vert", "shaders/skybox.frag", ShaderProgram::Type::SKYBOX));

    first_scene->createSkybox("skybox", ResourceManager::loadCubemap("textures/skybox"));

    first_scene->createCylindricalBillboard("pepeboard", "textures/pepe.png", glm::vec3(-4.0f, 2.0f, 0.0f));
    first_scene->createSphericalBillboard("containerboard", "textures/container.png", glm::vec3(3.0f, 3.0f, -3.0f), glm::vec2(2.0f));

    auto catcube = first_scene->createRigidBody("catcube", "objects/catcube/catcube.obj");
    auto catsphere = first_scene->createRigidBody("catsphere", "objects/catsphere/catsphere.obj", glm::vec3(-5.0f, 0.0f, 0.0f));

    first_scene->createDirectionalLight("dir_light0");
    first_scene->createPointLight("point_light0", glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    first_scene->createSpotLight("spot_light0", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.5f, 1.0f));
    
    Input::mapAction("forward", GLFW_KEY_U);
    Input::mapAction("left", GLFW_KEY_H);
    Input::mapAction("back", GLFW_KEY_J);
    Input::mapAction("right", GLFW_KEY_K);

    float catsphere_velocity = 5.0f;
    glm::vec3 catsphere_movement;

    // Main loop
    while (engine.isRunning())
    {
        catsphere_movement = glm::vec3(0.0f);

        if (Input::isActionPressed("forward"))
            catsphere_movement.z -= catsphere_velocity;
        if (Input::isActionPressed("left"))
            catsphere_movement.x -= catsphere_velocity;
        if (Input::isActionPressed("back"))
            catsphere_movement.z += catsphere_velocity;
        if (Input::isActionPressed("right"))
            catsphere_movement.x += catsphere_velocity;

        catsphere->translate(catsphere_movement * engine.getDeltaTime());

        catcube->rotate(glm::vec3(0.0f, 1.0f * engine.getDeltaTime(), 0.0f));

        engine.process();
    }

    return 0;
}