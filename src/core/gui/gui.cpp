#include "core/gui/gui.h"

void GUI::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    glfwGetFramebufferSize(window, &m_EngineFramebufferWidth, &m_EngineFramebufferHeight);

    m_EngineWindows.push_back(std::make_shared<qtzl::EngineWindow>(qtzl::EngineWindowType::SceneCfg));
    m_EngineWindows.push_back(std::make_shared<qtzl::EngineWindow>(qtzl::EngineWindowType::NodeMgr));
    m_EngineWindows.push_back(std::make_shared<qtzl::EngineWindow>(qtzl::EngineWindowType::ResourceMgr));
}

void GUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::render(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time)
{
    //m_AllSceneNodes = current_scene.second->getSceneNodeVec();

    // Engine windows
    for (const auto& window : m_EngineWindows)
    {
        if (!window->isVisible())
            continue;

        switch (window->getType())
        {
        case qtzl::EngineWindowType::SceneCfg:
            showSceneConfig(scene_name, scene, delta_time);
            break;
        case qtzl::EngineWindowType::NodeMgr:
            showNodeManager(scene_name, scene);
            break;
        case qtzl::EngineWindowType::ResourceMgr:
            showResourceManager();
            break;
        }
    }

    // Node windows
    for (const auto& window : m_NodeWindows)
    {
        window->render();
    }
}

void GUI::showSceneConfig(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time)
{
    ImGui::Begin((scene_name + " config").c_str(), 0);

    ImGui::Checkbox("Physics Enabled", &scene->m_IsPhysicsProcessing);

    ImGui::Separator();

    if (ImGui::Checkbox("Postprocessing Enabled", &scene->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    if (scene->m_IsPostProcessing)
    {
        std::vector<std::string> enabledShaders = scene->m_PostProcessing.getEnabledPPShaders(); // current screen shader names

        for (const auto& name : scene->m_PostProcessing.getPPShaderNames())
        {
            auto it = std::find(enabledShaders.begin(), enabledShaders.end(), name);

            if (ImGui::Selectable(name.substr(name.find("||") + 2).c_str(), it != enabledShaders.end()))
            {
                scene->m_PostProcessing.setPPShaderEnabled(name, it == enabledShaders.end());
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - 64); ImGui::Text((it != enabledShaders.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    // TODO: tools to create nodes on the fly

    ImGui::SeparatorText("Engine");

    // VSync
    ImGui::Checkbox("VSync Enabled", &m_EngineVSyncCur);
    if (m_EngineVSyncCur != m_EngineVSyncOld)
    {
        m_EngineVSyncOld = m_EngineVSyncCur;
        glfwSwapInterval(m_EngineVSyncCur);
    }

    // Camera config
    ImGui::DragFloat("Cam speed", &scene->m_Camera.m_Speed, 0.1f, 0.1f, 1000.0f, "%.1f");
    ImGui::DragFloat("Cam sens", &scene->m_Camera.m_Sensitivity, 0.1f, 0.1f, 1000.0f, "%.2f");

    ImGui::Separator();

    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_Position.x, scene->m_Camera.m_Position.y, scene->m_Camera.m_Position.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_Orientation.x, scene->m_Camera.m_Orientation.y, scene->m_Camera.m_Orientation.z);
    ImGui::Text("Framebuffer size: %ux%u", m_EngineFramebufferWidth, m_EngineFramebufferHeight);
    ImGui::Text("%.3f ms (%.1f FPS)", delta_time * 1000.0f, 1.0f / delta_time);

    ImGui::End();
}

void GUI::showNodeManager(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
    ImGui::Begin((scene_name + " node mgr").c_str(), 0);

    ImGui::Checkbox("Show Class Names", &m_NodeMgrShowClassNames);

    // Scene nodes list
    ImGui::Text("Current Nodes");
    if (ImGui::BeginListBox("##nodes", ImVec2(-FLT_MIN, std::min((int)scene->getNodes().size(), 10) * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (const auto& node_sptr : scene->getNodes())
        {
            std::string displayName = node_sptr->getName();

            if (m_NodeMgrShowClassNames)
            {
                std::string className = typeid(*node_sptr).name();
                displayName = className.substr(className.find_last_of("::") + 1) + " :: " + displayName;
            }

            if (ImGui::Selectable(displayName.c_str(), m_EngineCurrentNode_sptr == node_sptr))
            {
                m_EngineCurrentNode_sptr = node_sptr;
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    // Selected node config
    if (m_EngineCurrentNode_sptr != nullptr)
    {
        ImGui::SeparatorText(m_EngineCurrentNode_sptr->getName().c_str());

        // TODO: do smth like "std::map<std::string, std::any/std::optional> Node::getData()" to configure here
    }

    ImGui::End();
}

void GUI::showResourceManager()
{
    ImGui::Begin("Resource manager", 0);

    ImGui::SeparatorText("Meshes");
    for (const auto& entry : ResourceManager::getMeshes())
        ImGui::Text(entry.first.c_str());

    ImGui::SeparatorText("Textures");
    for (const auto& entry : ResourceManager::getTextures())
        ImGui::Text(entry.first.c_str());

    ImGui::SeparatorText("Shaders");
    for (const auto& entry : ResourceManager::getShaders())
        ImGui::Text(entry.first.c_str());

    ImGui::Separator();

    // TODO: tools to load resources on the fly

    ImGui::End();
}

// Making window using node3D
void GUI::onClick(const std::shared_ptr<qtzl::Node3D> node)
{
    // Selecting and deselecting
    auto window_it = std::find_if(m_NodeWindows.begin(), m_NodeWindows.end(), [&node](std::shared_ptr<qtzl::NodeWindow> window) { return window->getNode() == node; });

    if (window_it == m_NodeWindows.end())
        m_NodeWindows.push_back(std::make_shared<qtzl::NodeWindow>(node));
    else
        m_NodeWindows.erase(window_it);
}

bool GUI::isOccupied(double x, double y)
{
    for (const auto& window : m_NodeWindows)
    {
        if (window->getPosition().x > x && x > window->getPosition().x + window->getSize().x && window->getPosition().y > y && y > window->getPosition().y + window->getSize().y)
            return true;
    }

    for (const auto& window : m_EngineWindows)
    {
        if (!window->isVisible())
            continue;

        if (window->getPosition().x > x && x > window->getPosition().x + window->getSize().x && window->getPosition().y > y && y > window->getPosition().y + window->getSize().y)
            return true;
    }

    return false;
}

void GUI::updateFramebufferSize(int width, int height)
{
    m_EngineFramebufferWidth = width;
    m_EngineFramebufferHeight = height;
}

std::vector<std::shared_ptr<qtzl::NodeWindow>> GUI::getNodeWindows()
{
    return m_NodeWindows;
}