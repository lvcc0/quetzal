#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
    // GLFW stuff
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

Engine& Engine::instance(unsigned int width, unsigned int height)
{
    static Engine engine;
    engine.winHeight = height;
    engine.winWidth = width;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSwapInterval(true); // turn VSync on by default :)

    engine.createWindow();

    gladLoadGL();
    stbi_set_flip_vertically_on_load(true);
    
    GUI::init(engine.window);

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Stencil testing
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

    // Face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return engine;
}

bool Engine::isRunning() const
{
    return !glfwWindowShouldClose(this->window);
}

float Engine::getDeltaTime() const
{
    return this->deltaTime;
}

float Engine::getLastFrame() const
{
    return this->lastFrame;
}

void Engine::createWindow()
{
    this->window = glfwCreateWindow(winWidth, winHeight, "quetzal", NULL, NULL);

    if (this->window == NULL)
    {
        std::cerr << "ERROR::Engine::createWindow: can't create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));

    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallbackWrapper);
    glfwSetKeyCallback(this->window, keyCallbackWrapper);
    glfwSetMouseButtonCallback(window, mouseButtonCallbackWraper);
}

void Engine::processInput()
{
    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);

    if (!this->scenes.empty() && this->scenes.count(this->currentScene) && !this->scenes.at(this->currentScene)->m_Camera.m_Locked)
        this->scenes.at(this->currentScene)->m_Camera.processInput(this->window, deltaTime);
}

std::shared_ptr<Scene> Engine::createScene(const std::string& name, bool set_current)
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(this->winWidth, this->winHeight);

    // if the scene list is empty then we force set the first created one to be selected
    if (set_current || this->scenes.empty())
        this->currentScene = name;

    this->scenes.emplace(name, scene);

    return scene;
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    GUI::updateFramebufferSize(width, height);

    glfwGetWindowSize(window, &winWidth, &winHeight);

    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->m_Camera.updateSize(width, height);
        this->scenes.at(this->currentScene)->m_PostProcessing.recreate(width, height);
    }
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        this->shouldDrawGui = !this->shouldDrawGui;

    if (key >= 0 && key < 1024)
    {
        switch (action)
        {
        case GLFW_PRESS:
            Input::press(key);
            break;
        case GLFW_RELEASE:
            Input::release(key);
            break;
        }
    }
}

void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void Engine::process()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (this->shouldDrawGui)
    {
        GUI::render(currentScene, scenes, this->deltaTime);
        scenes.at(currentScene)->getNodeContainer().performActions(guiVisitor);
    }

    float curFrame = (float)glfwGetTime();
    this->deltaTime = curFrame - lastFrame;
    this->lastFrame = curFrame;

    this->processInput();

    // Update current scene here
    if (this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->update();

        // we don't wanna render the window if it isn't open
        if (winWidth > 0 && winHeight > 0) 
        {
            Renderer::renderBegin(this->scenes.at(this->currentScene));
            this->scenes.at(this->currentScene)->getNodeContainer().performActions(rendererVisitor);
            Renderer::renderEnd(this->scenes.at(this->currentScene));
        }

        // Facing billboards towards to the camera
        cameraVisitor.setCameraState(scenes.at(currentScene)->m_Camera);
        scenes.at(currentScene)->getNodeContainer().performActions(cameraVisitor);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height)
{
    Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));

    if (engine)
        engine->framebufferSizeCallback(window, width, height);
}

static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));

    if (engine)
        engine->keyCallback(window, key, scancode, action, mods);
}

void mouseButtonCallbackWraper(GLFWwindow* window, int key, int action, int mods)
{
    Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));

    if (engine)
        engine->mouseButtonCallback(window, key, action, mods);
}