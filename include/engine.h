#pragma once

#include "debugger.h"
#include "scene.h"
#include "gui.h"

#include "deque"

class Engine
{
public:
    GLFWwindow* window;

    std::map<const std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene;

    GUI m_GUI;

    // Constructor
    Engine(unsigned int width, unsigned int height);

    // Delete move and copy constructors
    Engine(const Engine& obj) = delete;
    Engine(const Engine&& obj) = delete;

    // Destructor
    ~Engine();

    bool isRunning() const;

    float getDeltaTime() const;
    float getLastFrame() const;

    void createWindow(); // create the glfw window
    void processInput(); // gets called every frame in the process() function below

    // Picking an object
    void pickObject();

    // Callbacks
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Main loop function
    void process();

    // Create a scene, add it to the scenes map and set currentScene to it
    std::shared_ptr<Scene> createScene(std::string name);

private:
    bool shouldDrawGui = false;

    unsigned int winWidth;
    unsigned int winHeight;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

// Callback static wrappers (because glfw doesn't know objects)
static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);