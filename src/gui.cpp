#include "gui.h"

GUI::GUI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
    // ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::guiLoop(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene)
{
    showCurrentSceneGUI(delta_time, current_scene);

    for (auto item : m_WindowsVec)
        item->windowLoop();
}

void GUI::showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene)
{
    ImGui::Begin((current_scene.first + " config").c_str());

    ImGui::Checkbox("Physics Enabled", &current_scene.second->m_IsPhysics);

    ImGui::Separator();

    if (ImGui::Checkbox("Postprocessing Enabled", &current_scene.second->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    if (current_scene.second->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> names = current_scene.second->m_PostProcessing->getScreenShaders(); // current screen shader names

        for (const auto& entry : current_scene.second->m_PostProcessing->m_ShaderMap)
        {
            auto it = std::find(names.begin(), names.end(), entry.first);

            if (ImGui::Selectable(entry.first.c_str(), (it != names.end())))
            {
                current_scene.second->m_PostProcessing->setScreenShader(entry.first, (it == names.end()));
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - 64); ImGui::Text((it != names.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    ImGui::SeparatorText("Create models");

    for (const auto& entry : ResourceManager::takeModels())
    {
        ImGui::Separator();
        std::string name = "make " + entry.first;
        if (ImGui::Selectable(name.c_str()))
        {
            current_scene.second->addModel(entry.first.c_str());
        }
    }

    ImGui::SeparatorText("Objects");

    // SceneNode UI
    if (!current_scene.second->getSceneNodeVec().empty())
    {
        ImGui::SeparatorText("Objects");

        for (const auto& entry : current_scene.second->getSceneNodeVec())
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry->getName().c_str()))
            {
                clickWindow(entry);
            }
        }
    }

    ImGui::SeparatorText("Engine");

    // TODO: rewrite this stuff more compact
    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", current_scene.second->m_Camera->m_pos.x, current_scene.second->m_Camera->m_pos.y, current_scene.second->m_Camera->m_pos.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", current_scene.second->m_Camera->m_orientation.x, current_scene.second->m_Camera->m_orientation.y, current_scene.second->m_Camera->m_orientation.z);
    ImGui::Text("%.3f ms (%.1f FPS)", delta_time * 1000.0f, 1.0f / delta_time);
    ImGui::Text("Pos X%.2f Y%.2f Size X%.2f Y%.2f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

    // This must be between render and end funcs
    GUIWindowPos = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    GUIWindowSize = glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    // ------------------------- //

    ImGui::End();
}

// Making window using scene node
void GUI::clickWindow(const std::shared_ptr<Scene_Node> obj_in)
{
    // Selecting and deselecting
    auto window_it = std::find_if(m_WindowsVec.begin(), m_WindowsVec.end(), [&obj_in](std::shared_ptr<GUI_Window_object_properties>obj) {if (obj->getNode() == obj_in) return true; return false; });
    
    if (window_it == m_WindowsVec.end())
        m_WindowsVec.push_back(std::make_shared<GUI_Window_object_properties>(obj_in));
    else
        m_WindowsVec.erase(window_it);
}

// Making window using renderable and convert renderable to scene_node
void GUI::clickWindow(const std::shared_ptr<Renderable> obj_in) 
{
    std::shared_ptr<Scene_Node> aux{ nullptr };
    if (typeid(*obj_in) == typeid(RigidBody))
    {
        auto temp = std::dynamic_pointer_cast<RigidBody>(obj_in);
        aux = std::dynamic_pointer_cast<Scene_Node>(temp);
    }
    else if (typeid(*obj_in) == typeid(CylindricalBillboard))
    {
        auto temp = std::dynamic_pointer_cast<CylindricalBillboard>(obj_in);
        aux = std::dynamic_pointer_cast<Scene_Node>(temp);
    }
    else if (typeid(*obj_in) == typeid(SphericalBillboard))
    {
        auto temp = std::dynamic_pointer_cast<SphericalBillboard>(obj_in);
        aux = std::dynamic_pointer_cast<Scene_Node>(temp);
    }
    else if (typeid(*obj_in) == typeid(Model))
    {
        auto temp = std::dynamic_pointer_cast<Model>(obj_in);
        aux = std::dynamic_pointer_cast<Scene_Node>(temp);
    }
    else
    {
        std::cout << "ERROR::GUI:CLICK_WINDOW HAVEN`T RECOGNISED INPUT OBJECT\n";
        __debugbreak();
    }
    // Selecting and deselecting
    auto window_it = std::find_if(m_WindowsVec.begin(), m_WindowsVec.end(), [&aux](std::shared_ptr<GUI_Window_object_properties>obj) {if (obj->getNode() == aux) return true; return false; });

    if (window_it == m_WindowsVec.end())
        m_WindowsVec.push_back(std::make_shared<GUI_Window_object_properties>(aux));
    else
        m_WindowsVec.erase(window_it);
}

GUI_Window_object_properties::GUI_Window_object_properties(const std::shared_ptr<Scene_Node> obj):
    m_SceneNode(obj)
{

};

GUI_Window_object_properties::~GUI_Window_object_properties()
{
    if (typeid(*m_SceneNode) == typeid(RigidBody))
    {
        auto temp = std::dynamic_pointer_cast<RigidBody>(m_SceneNode);
        temp->is_selected = false;
    }
    else if (typeid(*m_SceneNode) == typeid(CylindricalBillboard))
    {
        auto temp = std::dynamic_pointer_cast<CylindricalBillboard>(m_SceneNode);
        temp->is_selected = false;
    }
    else if (typeid(*m_SceneNode) == typeid(SphericalBillboard))
    {
        auto temp = std::dynamic_pointer_cast<SphericalBillboard>(m_SceneNode);
        temp->is_selected = false;
    }
    else if (typeid(*m_SceneNode) == typeid(Model))
    {
        auto temp = std::dynamic_pointer_cast <Model>(m_SceneNode);
        temp->is_selected = false;
    }
}

void GUI_Window_object_properties::windowLoop()
{
    if (typeid(*m_SceneNode) == typeid(RigidBody))
        showCurrentObjectGUI<RigidBody>();
    else if (typeid(*m_SceneNode) == typeid(CylindricalBillboard))
        showCurrentObjectGUI<CylindricalBillboard>();
    else if (typeid(*m_SceneNode) == typeid(SphericalBillboard))
        showCurrentObjectGUI<SphericalBillboard>();
    else if (typeid(*m_SceneNode) == typeid(Model))
        showCurrentObjectGUI<Model>();
}
