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

    // Renderable UI
    if (!current_scene.second->getRenderableVec().empty())
    {
        ImGui::SeparatorText("Renderables");

        for (const auto& entry : current_scene.second->getRenderableVec())
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

    ImGui::End();
}

void GUI::clickWindow(const std::shared_ptr<Renderable> obj_in)
{
    auto window_it = std::find_if(m_WindowsVec.begin(), m_WindowsVec.end(), [&obj_in](std::shared_ptr<GUI_Window_object_properties>obj) {if (obj->getRenderable() == obj_in) return true; return false; });
    if (window_it == m_WindowsVec.end())
        m_WindowsVec.push_back(std::make_shared<GUI_Window_object_properties>(obj_in));
    else
        m_WindowsVec.erase(window_it);
}

GUI_Window_object_properties::GUI_Window_object_properties(const std::shared_ptr<Renderable> obj):
    m_Renderable(obj)
{

}

GUI_Window_object_properties::~GUI_Window_object_properties()
{
    m_Renderable->is_selected = false;
}

void GUI_Window_object_properties::windowLoop()
{
    m_Renderable->is_selected = true;

    if (typeid(*m_Renderable) == typeid(RigidBody))
        showCurrentObjectGUI<RigidBody>(m_Renderable);
    else if (typeid(*m_Renderable) == typeid(CylindricalBillboard))
        showCurrentObjectGUI<CylindricalBillboard>(m_Renderable);
    else if (typeid(*m_Renderable) == typeid(SphericalBillboard))
        showCurrentObjectGUI<SphericalBillboard>(m_Renderable);
    else if (typeid(*m_Renderable) == typeid(Model))
        showCurrentObjectGUI<Model>(m_Renderable);
}
