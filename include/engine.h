#pragma once

#include "deque"

#include "scene.h"
#include "gui.h"

// Singleton
class Engine
{
private:
    Engine();
    ~Engine();

public:
    Engine(const Engine&) = delete; // no copying
    Engine(Engine&&) = delete;      // no moving
    Engine& operator= (const Engine&) = delete; // delete copy operator
    Engine& operator= (Engine&&) = delete;      // delete move operator
    
    static Engine& instance(unsigned int width, unsigned int height); 

    GLFWwindow* window;

    std::map<const std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene;

    bool isRunning() const;

    float getDeltaTime() const;
    float getLastFrame() const;

    void createWindow(); // create the glfw window
    void processInput(); // gets called every frame in the process() function below

    // Picking an object
    void pickObject(); // TODO

    // Callbacks
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // Main loop function
    void process();

    // Create a scene, add it to the scenes map and set currentScene to it
    std::shared_ptr<Scene> createScene(const std::string& name, bool set_current = true); // TODO: setting current scene should be optional

    // Set current scene and TODO: send some stuff to the renderer and such
    void setCurrentScene(const std::string& name);

private:
    std::unique_ptr<GUI> gui;
    
    bool shouldDrawGui = true;

    unsigned int winWidth;
    unsigned int winHeight;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

// Callback static wrappers (because glfw doesn't know objects)
static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseButtonCallbackWraper(GLFWwindow* window, int key, int action, int mods);