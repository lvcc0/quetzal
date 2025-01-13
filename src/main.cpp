#include "core/engine.h"

int main()
{
    Engine& engine = Engine::instance(1280, 720);
    ResourceManager::preloadResources();

    auto first_scene = engine.createScene("first_scene");

    std::shared_ptr<ShaderProgram> default_shader = ResourceManager::createShaderProgram("shaders/default.vert", "shaders/default.frag");
    Renderer::setCurrentShaderProgram(default_shader);

    first_scene->createCylindricalBillboard("pepeboard", "textures/pepe.png", glm::vec3(-4.0f, 2.0f, 0.0f));
    first_scene->createSphericalBillboard("containerboard", "textures/container.png", glm::vec3(3.0f, 3.0f, -3.0f), glm::vec2(2.0f));

    auto catcube = first_scene->createStaticBody("catcube", "objects/catcube/catcube.obj");
    first_scene->createStaticBody("catsphere", "objects/catsphere/catsphere.obj", glm::vec3(-5.0f, 0.0f, 0.0f));

    first_scene->createDirectionalLight("dir_light0");
    first_scene->createPointLight("point_light0", glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    first_scene->createSpotLight("spot_light0", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.5f, 1.0f));

    // Main loop
    while (engine.isRunning())
    {
        catcube->setGlobalRotation(glm::vec3(0.0f, engine.getLastFrame(), 0.0f));

        engine.process();
    }

    return 0;
}