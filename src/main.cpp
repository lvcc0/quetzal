#include "lights.h"          //
#include "debugger.h"        // TODO: need to add these to engine class
#include "post_processing.h" //

#include "engine.h"

int main()
{
    const unsigned int WIN_WIDTH = 1280;
    const unsigned int WIN_HEIGHT = 720;

    Engine engine(WIN_WIDTH, WIN_HEIGHT);
    
    auto first_scene = engine.createScene("first_scene");
    first_scene->addModel("catcube", "objects/catcube/catcube.obj");
    first_scene->addCylBillboard("container_billboard", glm::vec3(-5.0f, -2.0f, 0.0f), glm::vec2(4.0f, 4.0f), "textures/container.png");
    first_scene->addSphBillboard("pepeboard", glm::vec3(5.0f, 4.0f, -2.0f), glm::vec2(7.5f, 5.0f), "textures/pepe.png");

    // Main loop
    while (!glfwWindowShouldClose(engine.window)) // TODO: add some kind of indicator inside of the Engine class
    {
        engine.process();
    }

    // TODO: move all this stuff below to the engine class (destructor perhaps)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(engine.window);
    glfwTerminate();

    return 0;
}
