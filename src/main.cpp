#include "core/engine.h"

// TODO: separate scene scripts to separate files or smth
// TODO: fix all the stuff below

int main()
{
    Engine& engine = Engine::instance(1280, 720);
    ResourceManager::preloadResources();

    auto first_scene = engine.createScene("first_scene");
    auto second_scene = engine.createScene("second_scene");

    Renderer::setCurrentShaderProgram(ResourceManager::createShaderProgram("shaders/default.vert", "shaders/default.frag"));
    Renderer::setCurrentShaderProgram(ResourceManager::createShaderProgram("shaders/skybox.vert", "shaders/skybox.frag", ShaderProgram::Type::SKYBOX));

    // --- first_scene stuff --- //

    first_scene->createSkybox("skybox", ResourceManager::loadCubemap("textures/skybox"));

    first_scene->createCylindricalBillboard("pepeboard", "textures/pepe.png", glm::vec3(-4.0f, 2.0f, 0.0f));
    first_scene->createSphericalBillboard("containerboard", "textures/container.png", glm::vec3(3.0f, 3.0f, -3.0f), glm::vec2(2.0f));

    auto catcube = first_scene->createRigidBody("catcube", "objects/catcube/catcube.obj");
    auto catsphere = first_scene->createRigidBody("catsphere", "objects/catsphere/catsphere.obj", glm::vec3(-5.0f, 0.0f, 0.0f));

    catcube->addChild(first_scene->createBoxCollision("catcube_collision", catcube->getGlobalPosition(), glm::vec3(2.0f)));
    catsphere->addChild(first_scene->createBoxCollision("catsphere_collision", catsphere->getGlobalPosition(), glm::vec3(2.0f)));

    first_scene->createDirectionalLight("dir_light0");
    first_scene->createPointLight("point_light0", glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    first_scene->createSpotLight("spot_light0", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.5f, 1.0f));
    
    Input::mapAction("forward", GLFW_KEY_U);
    Input::mapAction("left", GLFW_KEY_H);
    Input::mapAction("back", GLFW_KEY_J);
    Input::mapAction("right", GLFW_KEY_K);

    float catsphere_velocity = 5.0f;
    glm::vec3 catsphere_movement;

    // --- //

    // --- second_scene stuff --- //

    second_scene->createSkybox("skybox", ResourceManager::loadCubemap("textures/skybox"));

    auto floor = second_scene->createStaticBody("floor", "objects/catcube/catcube.obj", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(10.0f, 0.5f, 2.0f));
    auto floor_collision = second_scene->createBoxCollision("floor_collision", floor->getGlobalPosition(), floor->getScale());
    floor->addChild(floor_collision);

    auto player_cube = second_scene->createRigidBody("player_cube", "objects/catcube/catcube.obj", glm::vec3(0.0f, 2.0f, 0.0f));
    auto player_collision = second_scene->createBoxCollision("player_collision", player_cube->getGlobalPosition(), player_cube->getScale());
    player_cube->addChild(player_collision);

    float velocity = 2.0f;
    float gravity_acceleraion = 0.4f;
    float y_momentum = 0.0f;

    glm::vec3 movement;

    // --- //

    // Main loop
    while (engine.isRunning())
    {
        if (engine.currentScene == "first_scene")
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
        }
        else if (engine.currentScene == "second_scene")
        {
            movement = glm::vec3(0.0f);

            if (Physics::areColliding(player_collision, floor_collision))
                y_momentum = 0.0f;

            // gravity stuff
            movement.y -= y_momentum;
            y_momentum += gravity_acceleraion;
            if (y_momentum > 10.0f) y_momentum = 10.0f;

            if (Input::isActionPressed("left"))
                movement.x -= velocity;
            if (Input::isActionPressed("right"))
                movement.x += velocity;

            player_cube->translate(movement * engine.getDeltaTime());
        }

        engine.process();
    }

    return 0;
}