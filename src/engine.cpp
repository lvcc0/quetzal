#include "engine.h"

Engine::Engine(unsigned int width, unsigned int height)
{
    this->win_width = width;
    this->win_height = height;

    this->camera = std::make_shared<Camera>(width, height, glm::vec3(0.0f, 0.0f, 5.0f));

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    this->createWindow();

    gladLoadGL();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    stbi_set_flip_vertically_on_load(true);

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Stencil testing
    glEnable(GL_STENCIL);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Engine::createWindow()
{
    this->window = glfwCreateWindow(win_width, win_height, "quetzal", NULL, NULL);

    if (this->window == NULL)
    {
        std::cout << "can't create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);

    // TODO: fix these callbacks
    //glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
    //glfwSetKeyCallback(this->window, keyCallback);
}

// Gets called every frame
void Engine::processInput()
{
    this->camera->Inputs(this->window, deltaTime);

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

// Gets called upon window resize
void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    this->camera->UpdateSize(width, height);
}

// Gets called upon key press
void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // do something here :P
}

void Engine::process()
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float curFrame = (float)glfwGetTime();
    deltaTime = curFrame - lastFrame;
    lastFrame = curFrame;

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->processInput();

    // Update current scene here
    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
        this->scenes.at(this->currentScene)->update();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

std::shared_ptr<Scene> Engine::createScene(std::string name)
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->camera = this->camera; // i hate this right here

    this->currentScene = name; // don't forget to change the current scene

    this->scenes.emplace(name, scene);

    return scene;
}