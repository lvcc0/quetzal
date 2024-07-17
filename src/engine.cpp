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

    // ImGUI init
    m_GUI.initialize(window);

    stbi_set_flip_vertically_on_load(true);

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
}

Engine::~Engine()
{
    // ImGui stuff
    m_GUI.destroyGUI();

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

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        pickObject();
    }

    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

void Engine::pickObject()
{
    GLdouble mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    glm::vec3 cam_coords = this->scenes.at(currentScene)->m_Camera->m_pos;
    glm::vec3 direction = ExpMath::getGlobalCoordsFromScreen(mouse_x, mouse_y, this->scenes.at(currentScene)->m_Camera->m_width, this->scenes.at(currentScene)->m_Camera->m_height, this->scenes.at(currentScene)->m_ProjectionMatrix, this->scenes.at(currentScene)->m_Camera->getViewMatrix());
    
    Ray ray(cam_coords, direction);
    
    // At this section checking models of rigid bodies (two corners of model make square)
    std::vector <std::pair<std::pair<std::string, std::shared_ptr<RigidBody>>, GLfloat >> inter_rigid_body_vector; // pair (pair (string, RigidBody), float)

    for (auto item : this->scenes.at(currentScene)->getRigidBodyMap()) 
    {
        GLfloat intersection_distance;
        if (ray.TestRayOBBIntersection(ExpMath::makeAABB(item.second->m_Model->m_Vertices).first, ExpMath::makeAABB(item.second->m_Model->m_Vertices).second, item.second->m_Model->m_ModelMatrix, intersection_distance))
        {
            inter_rigid_body_vector.push_back(std::pair(std::pair(item.first, item.second), intersection_distance));
        }
    }
    
    // Picking object with lesser intersection_distance
    std::pair<std::string, std::shared_ptr<RigidBody>> curr_rigid_body_ptr = ExpMath::getItemWithMinimumFloat<std::pair<std::string, std::shared_ptr<RigidBody>>>(inter_rigid_body_vector).first;

    if (curr_rigid_body_ptr.second != nullptr) 
    {
        // Deselecting current object before (if it has sense)
        if (m_GUI.m_CurrentRigidBody.second != nullptr && m_GUI.m_CurrentRigidBody.second != curr_rigid_body_ptr.second)
        {
            m_GUI.m_CurrentRigidBody.second->m_Model->is_selected = false;
        }

        // Setting the current variable  for IMGUI
        m_GUI.m_CurrentRigidBody.first = curr_rigid_body_ptr.first;
        m_GUI.m_CurrentRigidBody.second = curr_rigid_body_ptr.second;

        // Selecting model
        m_GUI.m_CurrentRigidBody.second->m_Model->is_selected = true;
    }
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
    // GUI PART //
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (this->shouldDrawGui)
        this->m_GUI.showCurrentSceneGUI(this->deltaTime);

    m_GUI.mainGUILoop();
    // --------- //
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
        this->scenes.at(this->currentScene)->doProcessing();
        this->scenes.at(this->currentScene)->update();
    }

    // GUI PART // 
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // -------- //

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

std::shared_ptr<Scene> Engine::createScene(std::string name)
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Camera(this->winWidth, this->winHeight, glm::vec3(0.0f)));

    this->scenes.emplace(name, scene);
    this->m_GUI.scenes.emplace(name, scene);

    this->currentScene = name; // don't forget to change the current scene
    this->m_GUI.currentScene = name;

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