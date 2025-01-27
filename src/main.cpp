#include "core/engine.h"

// TODO: separate scene scripts to separate files or smth

// yeah it is supposed to be here
static void doCollisions(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second, glm::vec3& movement)
{
    auto collision = Physics::areColliding(first, second);

    if (std::get<0>(collision)) // collision detected
    {
        Physics::Direction direction = std::get<1>(collision);
        glm::vec3 difference = std::get<2>(collision);

        auto first_parent = first->getParent();

        switch (direction)
        {
        case Physics::Direction::RIGHT:
        {
            movement.x = glm::min(movement.x, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") - glm::vec3(first->getVec3("Size").x / 2.0f - std::abs(difference.x), 0.0f, 0.0f));
            break;
        }
        case Physics::Direction::LEFT:
        {
            movement.x = glm::max(movement.x, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") + glm::vec3(first->getVec3("Size").x / 2.0f - std::abs(difference.x), 0.0f, 0.0f));
            break;
        }
        case Physics::Direction::UP:
        {
            movement.y = glm::min(movement.y, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") - glm::vec3(0.0f, first->getVec3("Size").y / 2.0f - std::abs(difference.y), 0.0f));
            break;
        }
        case Physics::Direction::DOWN:
        {
            movement.y = glm::max(movement.y, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") + glm::vec3(0.0f, first->getVec3("Size").y / 2.0f - std::abs(difference.y), 0.0f));
            break;
        }
        case Physics::Direction::FRONT:
        {
            movement.z = glm::min(movement.z, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") - glm::vec3(0.0f, 0.0f, first->getVec3("Size").z / 2.0f - std::abs(difference.z)));
            break;
        }
        case Physics::Direction::BACK:
        {
            movement.z = glm::max(movement.z, 0.0f);
            first_parent->set("Global position", first_parent->getVec3("Global position") + glm::vec3(0.0f, 0.0f, first->getVec3("Size").z / 2.0f - std::abs(difference.z)));
            break;
        }
        } // switch (direction)    
    }
}

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
    auto second_catcube = first_scene->createRigidBody("second_catcube", "objects/catcube/catcube.obj", glm::vec3(-3.0f, 0.0f, 0.0f));

    auto catcube_collision = first_scene->createBoxCollision("catcube_collision", catcube->getGlobalPosition(), glm::vec3(1.0f));
    auto second_catcube_collision = first_scene->createBoxCollision("catsphere_collision", second_catcube->getGlobalPosition(), glm::vec3(1.0f));

    catcube->addChild(catcube_collision);
    second_catcube->addChild(second_catcube_collision);

    first_scene->createDirectionalLight("dir_light0");
    first_scene->createPointLight("point_light0", glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    first_scene->createSpotLight("spot_light0", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.5f, 1.0f));
    
    Input::mapAction("forward", GLFW_KEY_U);
    Input::mapAction("left", GLFW_KEY_H);
    Input::mapAction("back", GLFW_KEY_J);
    Input::mapAction("right", GLFW_KEY_K);

    float catcube_velocity = 5.0f;
    glm::vec3 catcube_movement;

    // --- //

    // Main loop
    while (engine.isRunning())
    {
        if (engine.currentScene == "first_scene")
        {
            catcube_movement = glm::vec3(0.0f);

            if (Input::isActionPressed("forward"))
                catcube_movement.z -= catcube_velocity;
            if (Input::isActionPressed("left"))
                catcube_movement.x -= catcube_velocity;
            if (Input::isActionPressed("back"))
                catcube_movement.z += catcube_velocity;
            if (Input::isActionPressed("right"))
                catcube_movement.x += catcube_velocity;

            doCollisions(catcube_collision, second_catcube_collision, catcube_movement);

            catcube->translate(catcube_movement * engine.getDeltaTime());
        }

        engine.process();
    }

    return 0;
}