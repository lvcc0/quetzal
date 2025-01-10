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
        std::cout << "can't create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));

    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallbackWrapper);
    glfwSetKeyCallback(this->window, keyCallbackWrapper);
    glfwSetMouseButtonCallback(window, mouseButtonCallbackWraper);
}

// Gets called every frame
void Engine::processInput()
{
    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);

    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
        this->scenes.at(this->currentScene)->m_Camera.processInput(this->window, deltaTime);
}

// Badly working with zero scaled objects
void Engine::pickObject()
{
    // TODO: yeeeah
    // 
    //GLdouble mouse_x, mouse_y;
    //glfwGetCursorPos(window, &mouse_x, &mouse_y);

    //// Blocking that part of screen which is already occupied by:
    //// Main GUI of scene
    //// Main GUI contains more than one window
    //for (int i{ 0 }; i < gui->getGUIWindowPos().size(); i++)
    //{
    //    if (gui->getGUIWindowPos()[i].x + gui->getGUIWindowSize()[i].x > mouse_x && mouse_x > gui->getGUIWindowPos()[i].x
    //        && gui->getGUIWindowPos()[i].y + gui->getGUIWindowSize()[i].y > mouse_y && mouse_y > gui->getGUIWindowPos()[i].y)
    //        return;
    //}

    //// Window_obj_props
    //for (auto item : gui->getWindowsObjProps())
    //{
    //    for (int i{ 0 }; i < item->getGUIWindowPos().size(); i++)
    //    {
    //        if (item->getGUIWindowPos()[i].x + item->getGUIWindowSize()[i].x > mouse_x && mouse_x > item->getGUIWindowPos()[i].x
    //            && item->getGUIWindowPos()[i].y + item->getGUIWindowSize()[i].y > mouse_y && mouse_y > item->getGUIWindowPos()[i].y)
    //            return;
    //    }
    //}
    //// ----------------------------- //

    //glm::vec3 cam_coords = this->scenes.at(currentScene)->m_Camera.m_pos;
    //glm::vec3 direction = ExpMath::getGlobalCoordsFromScreen(mouse_x, mouse_y, this->scenes.at(currentScene)->m_Camera.m_width, this->scenes.at(currentScene)->m_Camera.m_height, Renderer::currentProjectionMatrix, this->scenes.at(currentScene)->m_Camera.getViewMatrix());

    //Ray ray(cam_coords, direction);
    //
    //// At this section checking models of rigid bodies (two corners of model make square)
    //std::vector <std::pair<std::shared_ptr<Renderable>, GLfloat >> inter_render_body_vector;

    //for (auto item : this->scenes.at(currentScene)->getRenderableVec()) 
    //{
    //    GLfloat intersection_distance;
    //    if (ray.TestRayOBBIntersection(ExpMath::makeAABB(item->m_Vertices).first, ExpMath::makeAABB(item->m_Vertices).second, item->getModelMatrix(), intersection_distance))
    //    {
    //        inter_render_body_vector.push_back(std::pair(item, intersection_distance));
    //    }
    //}
    //
    //// Picking object with lesser intersection_distance
    //if (inter_render_body_vector.size() != 0) {
    //    std::shared_ptr<Renderable> curr_renderable_ptr = ExpMath::getItemWithMinimumFloat(inter_render_body_vector).first;

    //    if (curr_renderable_ptr != nullptr)
    //    {
    //        gui->clickWindow(curr_renderable_ptr);
    //    }
    //}
}

// Gets called upon window resize
void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->m_Camera.updateSize(width, height);
        this->scenes.at(this->currentScene)->m_PostProcessing.recreate(width, height);
    }
}

// Gets called upon key press
void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS) // debugging purposes
        for (const auto& node : this->scenes.at(this->currentScene)->getNodes())
            std::cout << node->getName() << ": " << node->getParent() << std::endl;

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        this->shouldDrawGui = !this->shouldDrawGui;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
}

void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        pickObject();
}

void Engine::process()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (this->shouldDrawGui)
        GUI::render(currentScene, scenes.at(currentScene), this->deltaTime);

    float curFrame = (float)glfwGetTime();
    this->deltaTime = curFrame - lastFrame;
    this->lastFrame = curFrame;

    this->processInput();

    glClearColor(0.207f, 0.207f, 0.207f, 1.0f);                                 // clearing stuff in the default framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //
    glStencilMask(0x00); // turn off writing to the stencil buffer
    
    // Update current scene here
    if (!this->scenes.empty() && this->scenes.count(this->currentScene))
    {
        this->scenes.at(this->currentScene)->update();
        Renderer::render(this->scenes.at(this->currentScene));
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

std::shared_ptr<Scene> Engine::createScene(const std::string& name, bool set_current)
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(this->winWidth, this->winHeight);

    this->scenes.emplace(name, scene);
    
    if (set_current)
        this->setCurrentScene(name);

    return scene;
}

void Engine::setCurrentScene(const std::string& name)
{
    this->currentScene = name;
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
