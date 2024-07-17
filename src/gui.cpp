#include "gui.h"

GUI::GUI()
{
    
}

void GUI::initialize(GLFWwindow* window)
{
    if (!is_Exist) 
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        is_Exist = true;
    }
    else 
    {
        std::cerr << "ERROR::GUI ALREADY INITIALIZATED" << std::endl;
    }
}

void GUI::showCurrentSceneGUI(GLfloat delta_time)
{
    ImGui::Begin((this->currentScene + " config").c_str());

    ImGui::Checkbox("Preworking Enabled", &this->scenes.at(this->currentScene)->m_IsPreworking);

    ImGui::Separator();

    ImGui::Checkbox("Physics Enabled", &this->scenes.at(this->currentScene)->m_IsPhysics);

    ImGui::Separator();

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
    ImGui::SeparatorText("Objects");

    // Rigid bodies UI
    if (!this->scenes.at(this->currentScene)->getRigidBodyMap().empty())
    {
        ImGui::SeparatorText("Rigid bodies");

        auto items = this->scenes.at(this->currentScene)->getRigidBodyMap();

        for (const auto& entry : items)
        {
            ImGui::Separator();
            if (ImGui::Selectable(entry.first.c_str()))
            {
                if (m_CurrentRigidBody.second == entry.second)
                {
                    m_CurrentRigidBody.first = "";
                    m_CurrentRigidBody.second = nullptr;
                }
                else
                {
                    m_CurrentRigidBody.first = entry.first;
                    m_CurrentRigidBody.second = entry.second;
                }
            }
        }
    }
    // Spherical billboards UI
    if (!this->scenes.at(this->currentScene)->getSphericalBiillboardMap().empty())
    {
        ImGui::SeparatorText("Spherical billboards");

        auto items = this->scenes.at(this->currentScene)->getSphericalBiillboardMap();

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
    if (!this->scenes.at(this->currentScene)->getCylindricalBillboardMap().empty())
    {
        ImGui::SeparatorText("Cylindrical billboards");

        auto items = this->scenes.at(this->currentScene)->getCylindricalBillboardMap();

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
    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", this->scenes.at(this->currentScene)->m_Camera->m_pos.x, this->scenes.at(this->currentScene)->m_Camera->m_pos.y, this->scenes.at(this->currentScene)->m_Camera->m_pos.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", this->scenes.at(this->currentScene)->m_Camera->m_orientation.x, this->scenes.at(this->currentScene)->m_Camera->m_orientation.y, this->scenes.at(this->currentScene)->m_Camera->m_orientation.z);
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

void GUI::mainGUILoop()
{
    if (m_CurrentRigidBody.second != nullptr)
        showCurrentRigidBodyGuiWindow();
    if (m_CurrentCylindricalBillboard.second != nullptr)
        showCurrentCylBillboard();
    if (m_CurrentSphericalBillboard.second != nullptr)
        showCurrentSphericalBillboard();
}

void GUI::destroyGUI()
{
    // ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
