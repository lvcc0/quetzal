#include "engine.h"

int main()
{
    const unsigned int WIN_WIDTH = 1280;
    const unsigned int WIN_HEIGHT = 720;

    Engine engine(WIN_WIDTH, WIN_HEIGHT);

    auto first_scene = engine.createScene("first_scene");

    first_scene->addShader("default_shader", "shaders/default.vert", "shaders/default.frag");

    first_scene->setShader("default_shader");
    first_scene->setScreenShader("inversion_color");

    auto catCube = first_scene->addModel("catcube", "objects/catcube/catcube.obj", CollisionType::SQUARE);
    auto secCatCube = first_scene->addModel("sec_catcube", "objects/catcube/catcube.obj", CollisionType::SQUARE);
    auto catSphere = first_scene->addModel("catsphere", "objects/catsphere/catsphere.obj", CollisionType::SQUARE);
    auto pepeBoard = first_scene->addSphBillboard("pepeboard", glm::vec3(5.0f, 4.0f, -2.0f), glm::vec2(7.5f, 5.0f), "textures/pepe.png");
    auto containerBillboard = first_scene->addCylBillboard("container_billboard", glm::vec3(-5.0f, -2.0f, 0.0f), glm::vec2(4.0f, 4.0f), "textures/container.png");

    first_scene->addDirLight(DirLight(
        true,
        "dirLight0",
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.8f, 0.8f, 0.8f),
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
        catCube->translate(glm::vec3(0.0, 2.2, 0.0));
        catCube->rotate(engine.getLastFrame() * 50, glm::vec3(1.0, 0.0, 0.0));

        //catSphere->rotate(engine.getLastFrame() * 15, glm::vec3(0.0f, 1.0f, 0.0f));

        pepeBoard->translate(glm::vec3(cos(engine.getLastFrame()) * 10.0f, 0.0f, sin(engine.getLastFrame()) * 10.0f)); // TODO: billboard sprite renders somewhat weirdly when camera.y = board.y

        engine.process();
    }
}