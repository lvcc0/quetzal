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

    ImGui::Checkbox("Preworking Enabled", &current_scene.second->m_IsPreworking);

    ImGui::Separator();

    ImGui::Checkbox("Physics Enabled", &current_scene.second->m_IsPhysics);

    ImGui::Separator();

    if (ImGui::Checkbox("Postprocessing Enabled", &current_scene.second->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    if (current_scene.second->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> names = current_scene.second->getScreenShaders(); // current screen shader names

        for (const auto& entry : current_scene.second->m_PostProcessing->m_ShaderMap)
        {
            auto it = std::find(names.begin(), names.end(), entry.first);

            if (ImGui::Selectable(entry.first.c_str(), (it != names.end())))
            {
                current_scene.second->setScreenShader(entry.first, (it == names.end()));
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - 64); ImGui::Text((it != names.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }
    ImGui::SeparatorText("Objects");

    // Rigid bodies UI
    if (!current_scene.second->getRigidBodyMap().empty())
    {
        ImGui::SeparatorText("Rigid bodies");

        auto items = current_scene.second->getRigidBodyMap();

        for (const auto& entry : items)
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry.first.c_str()))
            {
                if (m_CurrentRigidBody.second == entry.second)
                {
                    // Deselecting model
                    m_CurrentRigidBody.second->m_Model->is_selected = false;

                    m_CurrentRigidBody.first = "";
                    m_CurrentRigidBody.second = nullptr;
                }
                else
                {
                    // Deselecting model
                    if (m_CurrentRigidBody.second != nullptr)
                        m_CurrentRigidBody.second->m_Model->is_selected = false;

                    m_CurrentRigidBody.first = entry.first;
                    m_CurrentRigidBody.second = entry.second;

                    // Selecting model
                    m_CurrentRigidBody.second->m_Model->is_selected = true;
                }
            }
        }
    }
    // Spherical billboards UI
    if (!current_scene.second->getSphericalBiillboardMap().empty())
    {
        ImGui::SeparatorText("Spherical billboards");

        auto items = current_scene.second->getSphericalBiillboardMap();

        for (const auto& entry : items)
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry.first.c_str()))
            {
                if (m_CurrentSphericalBillboard.second == entry.second)
                {
                    m_CurrentSphericalBillboard.first = "";
                    m_CurrentSphericalBillboard.second = nullptr;
                }
                else
                {
                    m_CurrentSphericalBillboard.first = entry.first;
                    m_CurrentSphericalBillboard.second = entry.second;
                }
            }
        }
    }
    // Cylindrical billboards UI
    if (!current_scene.second->getCylindricalBillboardMap().empty())
    {
        ImGui::SeparatorText("Cylindrical billboards");

        auto items = current_scene.second->getCylindricalBillboardMap();

        for (const auto& entry : items)
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry.first.c_str()))
            {
                if (m_CurrentCylindricalBillboard.second == entry.second)
                {
                    m_CurrentCylindricalBillboard.first = "";
                    m_CurrentCylindricalBillboard.second = nullptr;
                }
                else
                {
                    m_CurrentCylindricalBillboard.first = entry.first;
                    m_CurrentCylindricalBillboard.second = entry.second;
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

void GUI::showCurrentRigidBodyGuiWindow()
{
    ImGui::Begin((m_CurrentRigidBody.first + " config").c_str());

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentRigidBody.second->m_Position, 0.5f);

    ImGui::Separator();
    ImGui::DragFloat3("Move speed", (float*)&m_CurrentRigidBody.second->m_MoveVector, 0.001f);

    ImGui::Separator();
    ImGui::DragFloat3("Scale", (float*)&m_CurrentRigidBody.second->m_Scale, 0.2f);

    ImGui::Separator();
    ImGui::DragFloat3("Rotation", (float*)&m_CurrentRigidBody.second->m_RotationDegrees, 1.0f);

    ImGui::End();
}

void GUI::showCurrentCylBillboard()
{
    ImGui::Begin((m_CurrentCylindricalBillboard.first + " config").c_str());

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentCylindricalBillboard.second->m_Position, 0.5f);

    ImGui::End();
}

void GUI::showCurrentSphericalBillboard()
{
    ImGui::Begin((m_CurrentSphericalBillboard.first + " config").c_str());

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentSphericalBillboard.second->m_Position, 0.5f);

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
    if (m_CurrentRigidBody.second != nullptr)
        showCurrentRigidBodyGuiWindow();
    if (m_CurrentCylindricalBillboard.second != nullptr)
        showCurrentCylBillboard();
    if (m_CurrentSphericalBillboard.second != nullptr)
        showCurrentSphericalBillboard();
}
