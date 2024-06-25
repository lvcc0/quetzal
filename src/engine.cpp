#include "engine.h"

Engine::Engine(unsigned int width, unsigned int height)
    : winWidth(width), winHeight(height)
{
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

Engine::~Engine()
{
    // ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLFW stuff
    glfwDestroyWindow(this->window);
    glfwTerminate();
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
        std::cout << "can't create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));

    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallbackWrapper);
    glfwSetKeyCallback(this->window, keyCallbackWrapper);
}

// Gets called every frame
void Engine::processInput()
{
    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
        this->scenes.at(this->currentScene)->m_Camera->Inputs(this->window, deltaTime);

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

void Engine::showGuiWindow()
{
    ImGui::Begin((this->currentScene + " config").c_str());

    if (ImGui::Checkbox("Postprocessing Enabled", &this->scenes.at(this->currentScene)->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    if (this->scenes.at(this->currentScene)->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> names = this->scenes.at(this->currentScene)->getScreenShaders(); // current screen shader names

        for (const auto& entry : this->scenes.at(this->currentScene)->m_PostProcessing->m_ShaderMap)
        {
            auto it = std::find(names.begin(), names.end(), entry.first);

            if (ImGui::Selectable(entry.first.c_str(), (it != names.end())))
            {
                this->scenes.at(this->currentScene)->setScreenShader(entry.first, (it == names.end()));
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - 64); ImGui::Text((it != names.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    ImGui::SeparatorText("Engine");

    // TODO: rewrite this stuff more compact
    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", this->scenes.at(this->currentScene)->m_Camera->m_pos.x, this->scenes.at(this->currentScene)->m_Camera->m_pos.y, this->scenes.at(this->currentScene)->m_Camera->m_pos.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", this->scenes.at(this->currentScene)->m_Camera->m_orientation.x, this->scenes.at(this->currentScene)->m_Camera->m_orientation.y, this->scenes.at(this->currentScene)->m_Camera->m_orientation.z);
    ImGui::Text("%.3f ms (%.1f FPS)", this->deltaTime * 1000.0f, 1.0f / this->deltaTime);

    ImGui::End();
}

// Gets called upon window resize
void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->m_Camera->UpdateSize(width, height);
        this->scenes.at(this->currentScene)->m_PostProcessing->recreate(width, height);
    }
}

// Gets called upon key press
void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        this->shouldDrawGui = !this->shouldDrawGui;
}

void Engine::process()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (this->shouldDrawGui)
        this->showGuiWindow();

    float curFrame = (float)glfwGetTime();
    this->deltaTime = curFrame - lastFrame;
    this->lastFrame = curFrame;

    this->processInput();

    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);                                 // clearing stuff in the default framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //

    // Update current scene here
    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->doPhysicsProcessing();
        this->scenes.at(this->currentScene)->update();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

std::shared_ptr<Scene> Engine::createScene(std::string name)
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Camera(this->winWidth, this->winHeight, glm::vec3(0.0f)));

    this->scenes.emplace(name, scene);
    this->currentScene = name; // don't forget to change the current scene

    return scene;
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