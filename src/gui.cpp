#include "gui.h"

void GUI::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    m_EngineWindows.push_back(std::make_shared<GUIEngineWindow>(EngineWindowType::SceneCfg));
    m_EngineWindows.push_back(std::make_shared<GUIEngineWindow>(EngineWindowType::NodeMgr));
    m_EngineWindows.push_back(std::make_shared<GUIEngineWindow>(EngineWindowType::ResourceMgr));
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

        switch (window->m_Type)
        {
        case EngineWindowType::SceneCfg:
            showSceneConfig(scene_name, scene, delta_time);
            break;
        case EngineWindowType::NodeMgr:
            showNodeManager(scene);
            break;
        case EngineWindowType::ResourceMgr:
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
    ImGui::Begin((scene_name + " config").c_str(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::Checkbox("Physics Enabled", &scene->m_IsPhysics);

    ImGui::Separator();

    if (ImGui::Checkbox("Postprocessing Enabled", &scene->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    if (scene->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> names = scene->m_PostProcessing.getScreenShaders(); // current screen shader names

        for (const auto& entry : scene->m_PostProcessing.m_ShaderMap)
        {
            auto it = std::find(names.begin(), names.end(), entry.first);

            if (ImGui::Selectable(entry.first.c_str(), (it != names.end())))
            {
                scene->m_PostProcessing.setScreenShader(entry.first, (it == names.end()));
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - 64); ImGui::Text((it != names.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    ImGui::SeparatorText("Objects");

    // SceneNode UI
    //if (!scene->getSceneNodeVec().empty())
    //{
    //    ImGui::SeparatorText("Objects");
    //
    //    for (const auto& entry : current_scene.second->getSceneNodeVec())
    //    {
    //        ImGui::Separator();
    //        if (ImGui::Selectable(entry->getName().c_str()))
    //        {
    //            clickWindow(entry);
    //        }
    //    }
    //}

    ImGui::SeparatorText("Engine");

    // TODO: rewrite this stuff more compact
    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_pos.x, scene->m_Camera.m_pos.y, scene->m_Camera.m_pos.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_orientation.x, scene->m_Camera.m_orientation.y, scene->m_Camera.m_orientation.z);
    ImGui::Text("%.3f ms (%.1f FPS)", delta_time * 1000.0f, 1.0f / delta_time);
    ImGui::Text("Pos X%.2f Y%.2f Size X%.2f Y%.2f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

    ImGui::End();
}

void GUI::showNodeManager(std::shared_ptr<Scene> scene)
{
    ImGui::Begin("Node manager", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SeparatorText("Create models");

    //for (const auto& entry : ResourceManager::getModels())
    //{
    //    ImGui::Separator();
    //    std::string name = "make " + entry.first;
    //    if (ImGui::Selectable(name.c_str()))
    //    {
    //        scene->addModel(entry.first.c_str());
    //    }
    //}

    ImGui::End();
}

void GUI::showResourceManager()
{
    // TODO: list all loaded resources here
}

// Making window using node
void GUI::onClick(const std::shared_ptr<qtzl::Node> obj)
{
    // Selecting and deselecting
    auto window_it = std::find_if(m_NodeWindows.begin(), m_NodeWindows.end(), [&obj](std::shared_ptr<GUINodeWindow> obj) { return obj->getNode() == obj; });
    
    if (window_it == m_NodeWindows.end())
        m_NodeWindows.push_back(std::make_shared<GUINodeWindow>(obj));
    else
        m_NodeWindows.erase(window_it);
}

// Making window using node3D and casting it to node
void GUI::onClick(const std::shared_ptr<qtzl::Node3D> obj)
{
    std::shared_ptr<qtzl::Node> aux = std::dynamic_pointer_cast<qtzl::Node>(obj);
    if (aux == nullptr)
        return;

    // Selecting and deselecting
    auto window_it = std::find_if(m_NodeWindows.begin(), m_NodeWindows.end(), [&aux](std::shared_ptr<GUINodeWindow>obj) { return obj->getNode() == aux; });

    if (window_it == m_NodeWindows.end())
        m_NodeWindows.push_back(std::make_shared<GUINodeWindow>(aux));
    else
        m_NodeWindows.erase(window_it);
}

bool GUI::isOccupied(double x, double y)
{
    for (const auto& window : m_NodeWindows)
    {
        if (window->getPosition().x > x > window->getPosition().x + window->getSize().x && window->getPosition().y > y > window->getPosition().y + window->getSize().y)
            return true;
    }

    for (const auto& window : m_EngineWindows)
    {
        if (window->getPosition().x > x > window->getPosition().x + window->getSize().x && window->getPosition().y > y > window->getPosition().y + window->getSize().y)
            return true;
    }

    return false;
}

std::vector<std::shared_ptr<GUINodeWindow>> GUI::getNodeWindows()
{
    return m_NodeWindows;
}

GUIWindow::GUIWindow()
{
}

GUIWindow::~GUIWindow()
{
}

GUIEngineWindow::GUIEngineWindow(EngineWindowType type)
    : m_Type(type)
{
}

bool GUIEngineWindow::isVisible() const
{
    return m_IsVisible;
}

void GUIEngineWindow::toggleVisibility()
{
    m_IsVisible = !m_IsVisible;
}

GUINodeWindow::GUINodeWindow(std::shared_ptr<qtzl::Node3D> node)
    : m_Node(node)
{
}

GUINodeWindow::~GUINodeWindow()
{
    // TODO: deselect window's node if it was selected
    //auto aux = std::dynamic_pointer_cast<qtzl::Node3D>(m_Node);
    //if (aux != nullptr)
    //    aux->is_selected = false;
}

void GUINodeWindow::render()
{
    // TODO: yeah
    //if (typeid(m_Node) == typeid(/*node*/))
    //    showNodeGUI</*node*/>();
    // and so on
}