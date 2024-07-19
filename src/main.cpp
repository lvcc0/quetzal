#include "engine.h"

int main()
{
    const unsigned int WIN_WIDTH = 1280;
    const unsigned int WIN_HEIGHT = 720;

    Engine& engine = Engine::Instance(WIN_WIDTH, WIN_HEIGHT);
    GUI& gui = GUI::Instance(engine.window);

    auto first_scene = engine.createScene("first_scene");

    first_scene->addShader("default_shader", "shaders/default.vert", "shaders/default.frag");
    first_scene->addShader("stencil_shader", "shaders/stencil.vert", "shaders/stencil.frag");

    auto catCube1 = first_scene->addRigidBody("catcube1", "objects/catcube/catcube.obj", Collision(CollisionType::BOX, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    auto catCube2 = first_scene->addRigidBody("catcube2", "objects/catcube/catcube.obj", Collision(CollisionType::BOX, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    auto catCube3 = first_scene->addRigidBody("catcube3", "objects/catcube/catcube.obj", Collision(CollisionType::BOX, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    auto catSphere1 = first_scene->addRigidBody("catsphere1", "objects/catsphere/catsphere.obj", Collision(CollisionType::BOX, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));

    auto pepeBoard = first_scene->addSphBillboard("pepeboard", glm::vec3(0.0f), glm::vec2(7.5f, 5.0f), "textures/pepe.png");
    auto containerBillboard = first_scene->addCylBillboard("container_billboard", glm::vec3(-5.0f, -2.0f, 0.0f), glm::vec2(4.0f, 4.0f), "textures/container.png");

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
    ));

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
    ));

    // Main loop
    while (engine.isRunning())
    {
        engine.process(gui);
    }
}