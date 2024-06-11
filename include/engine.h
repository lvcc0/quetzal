#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "debugger.h"
#include "scene.h"

class Engine
{
public:
    GLFWwindow* window;
    std::shared_ptr<Camera> camera;

    std::map<const std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene;

    // Constructor
    Engine(unsigned int width, unsigned int height);

    // Destructor
    ~Engine();

    void createWindow(); // create the glfw window
    void processInput(); // gets called every frame in the process() function below

    // Callbacks
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Main loop function
    void process();

    // Create a scene, add it to the scenes map and set currentScene to it
    std::shared_ptr<Scene> createScene(std::string name);

private:
    unsigned int win_width;
    unsigned int win_height;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};