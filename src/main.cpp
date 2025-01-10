#include "core/engine.h"

int main()
{
    const unsigned int WIN_WIDTH = 1280;
    const unsigned int WIN_HEIGHT = 720;
    
    Engine& engine = Engine::instance(WIN_WIDTH, WIN_HEIGHT);
    ResourceManager::preLoadResources();

    auto first_scene = engine.createScene("first_scene");

    std::shared_ptr<ShaderProgram> default_shader = ResourceManager::createShaderProgram("default.vert", "default.frag");
    Renderer::setCurrentShaderProgram(default_shader);

    first_scene->createCylindricalBillboard("pepeboard", "pepe.png");
    first_scene->createSphericalBillboard("containerboard", "container.png", glm::vec3(3.0f, 3.0f, -3.0f), glm::vec2(2.0f));

    first_scene->createDirectionalLight("dir_light0");
    first_scene->createPointLight("point_light0");
    first_scene->createSpotLight("spot_light0");

    // Main loop
    while (engine.isRunning())
    {
        engine.process();
    }

    return 0;
}