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

void GUI::guiLoop(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window)
{
    m_AllSceneNodes = current_scene.second->getSceneNodeVec();
    showCurrentSceneGUI(delta_time, current_scene, window);

    for (auto item : m_WindowsVec)
        item->windowLoop();
}

void GUI::showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window)
{
    // Delete vecs
    GUIWindowPos.clear();
    GUIWindowSize.clear();

    // First window //
    ImGui::Begin((current_scene.first + " config").c_str(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    positioningWindow(window, 0.3, 1, 0.7, 0);

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
    GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
    GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    // ------------- //

    ImGui::End();
    // ------------------------------------ //
    // Second window //
    ImGui::Begin("Make objects", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    positioningWindow(window, 0.7, 0.3, 0, 0.7);

    ImGui::SeparatorText("Create models");

    for (const auto& entry : ResourceManager::getModels())
    {
        ImGui::Separator();
        std::string name = "make " + entry.first;
        if (ImGui::Selectable(name.c_str()))
        {
            current_scene.second->addModel(entry.first.c_str());
        }
    }

    // This must be between render and end funcs
    GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
    GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    // ------------- //

    ImGui::End();
    // -------------- // 
}

// Making window using scene node
void GUI::clickWindow(const std::shared_ptr<Scene_Node> obj_in)
{
    // Selecting and deselecting
    auto window_it = std::find_if(m_WindowsVec.begin(), m_WindowsVec.end(), [&obj_in](std::shared_ptr<GUI_Window_object_properties>obj) {if (obj->getNode() == obj_in) return true; return false; });
    
    if (window_it == m_WindowsVec.end())
        m_WindowsVec.push_back(std::make_shared<GUI_Window_object_properties>(obj_in, m_AllSceneNodes));
    else
        m_WindowsVec.erase(window_it);
}

// Making window using renderable and convert renderable to scene_node
void GUI::clickWindow(const std::shared_ptr<Renderable> obj_in) 
{
    std::shared_ptr<Scene_Node> aux = std::dynamic_pointer_cast<Scene_Node>(obj_in);
    if (aux == nullptr)
        return;

    // Selecting and deselecting
    auto window_it = std::find_if(m_WindowsVec.begin(), m_WindowsVec.end(), [&aux](std::shared_ptr<GUI_Window_object_properties>obj) {if (obj->getNode() == aux) return true; return false; });

    if (window_it == m_WindowsVec.end())
        m_WindowsVec.push_back(std::make_shared<GUI_Window_object_properties>(aux, m_AllSceneNodes));
    else
        m_WindowsVec.erase(window_it);
}

GUI_Window_object_properties::GUI_Window_object_properties(const std::shared_ptr<Scene_Node> obj, const std::vector<std::shared_ptr<Scene_Node>> scene_nodes):
    m_SceneNode(obj), m_AllSceneNodes(scene_nodes)
{
    m_AllSceneNodes.erase(std::find(m_AllSceneNodes.begin(), m_AllSceneNodes.end(), obj));
};

GUI_Window_object_properties::~GUI_Window_object_properties()
{
    auto aux = std::dynamic_pointer_cast<Renderable>(m_SceneNode);
    if (aux != nullptr)
        aux->is_selected = false;
}

void GUI_Window_object_properties::windowLoop()
{
    GUIWindowPos.clear();
    GUIWindowSize.clear();
    if (typeid(*m_SceneNode) == typeid(CylindricalBillboard))
        showCurrentObjectGUI<CylindricalBillboard>();
    else if (typeid(*m_SceneNode) == typeid(SphericalBillboard))
        showCurrentObjectGUI<SphericalBillboard>();
    else if (typeid(*m_SceneNode) == typeid(Model))
        showCurrentObjectGUI<Model>();
}

template <typename T>
void GUI_Window_object_properties::showCurrentObjectGUI()
{
    ASSERT(false);
}
template<>
void GUI_Window_object_properties::showCurrentObjectGUI<Model>()
{
    std::shared_ptr<Model> m_CurrentModel = std::dynamic_pointer_cast<Model>(m_SceneNode);
    //m_CurrentModel->is_selected = true;

    ImGui::Begin((m_CurrentModel->getName() + " config").c_str(), 0, ImGuiWindowFlags_NoCollapse);

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentModel->m_Position, 0.5f);

    ImGui::Separator();
    ImGui::DragFloat3("Scale", (float*)&m_CurrentModel->m_Scale, 0.1f, 0.01, 1000);

    ImGui::Separator();
    ImGui::DragFloat3("Rotation", (float*)&m_CurrentModel->m_RotationDegrees, 1.0f, 0.0, 360);

    // Attaching object to another scene_node
    // Making special window
    ImGui::Separator();
    if (ImGui::Button("Attach"))
        attaching = !attaching;

    if (attaching == true)
    {
        ImGui::Begin((m_CurrentModel->getName() + " attaching").c_str(), 0, ImGuiWindowFlags_NoCollapse);
        
        ImGui::Separator();
        for (auto item : m_AllSceneNodes)
        {
            bool is_selected = false;
            if (m_SceneNode->getParent() == item)
                is_selected = true;

            if (ImGui::Selectable(item->getName().c_str(), is_selected))
            {
                m_SceneNode->setParent(item);
                ImGui::Separator();
            }
        }
        if (ImGui::Button("delete parent"))
        {
            m_CurrentModel->setParent(nullptr);
        }
        // This must be between render and end funcs
        GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
        GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
        // ------------------------- //

        ImGui::End();
    }
    // --------------------------------- //
    
    // This must be between render and end funcs
    GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
    GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    // ------------------------- //

    ImGui::End();
}
template<>
void GUI_Window_object_properties::showCurrentObjectGUI<CylindricalBillboard>()
{
    std::shared_ptr<CylindricalBillboard> m_CurrentCylBill = std::dynamic_pointer_cast<CylindricalBillboard>(m_SceneNode);
    m_CurrentCylBill->is_selected = true;

    ImGui::Begin((m_CurrentCylBill->getName() + " config").c_str(), 0, ImGuiWindowFlags_NoCollapse);

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill->m_Position, 0.5f);

    // This must be between render and end funcs
    GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
    GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    // ------------------------- //

    ImGui::End();
}
template<>
void GUI_Window_object_properties::showCurrentObjectGUI<SphericalBillboard>()
{
    std::shared_ptr<SphericalBillboard> m_CurrentSphBill = std::dynamic_pointer_cast<SphericalBillboard>(m_SceneNode);
    m_CurrentSphBill->is_selected = true;

    ImGui::Begin((m_CurrentSphBill->getName() + " config").c_str(), 0, ImGuiWindowFlags_NoCollapse);

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill->m_Position, 0.5f);

    // This must be between render and end funcs
    GUIWindowPos.push_back(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));
    GUIWindowSize.push_back(glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    // ------------------------- //

    ImGui::End();
}

template<class Type> requires constraint_window_properties<Type>
void WindowProperties<Type>::positioningWindow(GLFWwindow* window, float part_of_width, float part_of_height, float pos_x, float pos_y)
{
    int w_width, w_height;
    glfwGetWindowSize(window, &w_width, &w_height);

    ImGui::SetWindowSize(ImVec2(w_width * part_of_width, w_height * part_of_height));
    ImGui::SetWindowPos(ImVec2(w_width * pos_x, w_height * pos_y));
}
