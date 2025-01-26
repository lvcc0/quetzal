#pragma once

#include "static/gui.h"
#include "static/input.h"
#include "static/renderer.h"
#include "static/physics.h"

#include "core/scene.h"

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

    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene;

    bool isRunning() const;

    float getDeltaTime() const;
    float getLastFrame() const;

    // Create the glfw window
    void createWindow();

    // Gets called every frame in the Engine::process function
    void processInput(); 

    // Create a scene, add it to the scenes map and set currentScene if needed
    std::shared_ptr<Scene> createScene(const std::string& name, bool set_current = false);

    // Gets called upon window resize
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    // Gets called upon key press
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Gets called upon mouse click
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // Main loop function
    void process();

private:
    bool shouldDrawGui = true;

    int winWidth;
    int winHeight;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

// Callback static wrappers (because glfw doesn't know objects)

static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseButtonCallbackWraper(GLFWwindow* window, int key, int action, int mods);