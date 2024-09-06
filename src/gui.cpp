#include "gui.h"

GUI::GUI()
{
    
}

GUI::~GUI()
{
    // ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
    ImGui::SeparatorText("Objects");

    // Renderable UI
    if (!current_scene.second->getRenderableMap().empty())
    {
        ImGui::SeparatorText("Renderables");

        auto items = current_scene.second->getRenderableMap();

        for (const auto& entry : items)
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry.first.c_str()))
            {
                if (m_CurrentRenderable.second == entry.second)
                {
                    // Deselecting renderable
                    m_CurrentRenderable.second->is_selected = false;

                    m_CurrentRenderable.first = "";
                    m_CurrentRenderable.second = nullptr;
                }
                else
                {
                    // Deselecting renderable
                    if (m_CurrentRenderable.second != nullptr)
                        m_CurrentRenderable.second->is_selected = false;

                    m_CurrentRenderable.first = entry.first;
                    m_CurrentRenderable.second = entry.second;

                    // Selecting renderable
                    m_CurrentRenderable.second->is_selected = true;
                }
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

GUI& GUI::Instance(GLFWwindow* window)
{
    static GUI gui;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return gui;
}

void GUI::mainGUILoop()
{
    if (m_CurrentRenderable.second != nullptr) 
    {
        auto current_renderable = m_CurrentRenderable.second;
        if (typeid(*current_renderable) == typeid(RigidBody))
            showCurrentObjectGUI<RigidBody>();
        else if (typeid(*current_renderable) == typeid(CylindricalBillboard))
            showCurrentObjectGUI<CylindricalBillboard>();
        else if (typeid(*current_renderable) == typeid(SphericalBillboard))
            showCurrentObjectGUI<SphericalBillboard>();
        else if (typeid(*current_renderable) == typeid(Model))
        {
            // Nothing
        }
    }
    
}
