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
    ImGui::Begin((m_CurrentCylBill.first + " config").c_str());

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill.second->m_Position, 0.5f);

    ImGui::End();
}

void GUI::showCurrentSphericalBillboard()
{
    ImGui::Begin((m_CurrentSphBill.first + " config").c_str());

    ImGui::Separator();
    ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill.second->m_Position, 0.5f);

    ImGui::End();
}

void GUI::cleanCurrents()
{
    if (m_CurrentModel.second != nullptr)
    {
        m_CurrentModel.first = "";
        m_CurrentModel.second = nullptr;
    }
    if (m_CurrentRigidBody.second != nullptr)
    {
        m_CurrentRigidBody.first = "";
        m_CurrentRigidBody.second = nullptr;
    }
    if (m_CurrentCylBill.second != nullptr)
    {
        m_CurrentCylBill.first = "";
        m_CurrentCylBill.second = nullptr;
    }
    if (m_CurrentSphBill.second != nullptr)
    {
        m_CurrentSphBill.first = "";
        m_CurrentSphBill.second = nullptr;
    }
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
    cleanCurrents();
    if (m_CurrentRenderable.second != nullptr) 
    {
        switch (m_CurrentRenderable.second->type)
        {
        case(RenderableType::RIGID_BODY):
            m_CurrentRigidBody.first = m_CurrentRenderable.first;
            m_CurrentRigidBody.second = std::static_pointer_cast<RigidBody>(m_CurrentRenderable.second);

            break;
        case(RenderableType::MODEL):
            m_CurrentModel.first = m_CurrentRenderable.first;
            m_CurrentModel.second = std::static_pointer_cast<Model>(m_CurrentRenderable.second);

            break;
        case(RenderableType::CYL_BILL):
            m_CurrentCylBill.first = m_CurrentRenderable.first;
            m_CurrentCylBill.second = std::static_pointer_cast<CylindricalBillboard>(m_CurrentRenderable.second);

            break;
        case(RenderableType::SPH_BILL):
            m_CurrentSphBill.first = m_CurrentRenderable.first;
            m_CurrentSphBill.second = std::static_pointer_cast<SphericalBillboard>(m_CurrentRenderable.second);

            break;
        default:

            std::cerr << "ERROR::GUI::RENDERABLE_TYPE NOT FOUND" << std::endl;
            break;
        }
    }

    if (m_CurrentRigidBody.second != nullptr)
        showCurrentRigidBodyGuiWindow();
    if (m_CurrentCylBill.second != nullptr)
        showCurrentCylBillboard();
    if (m_CurrentSphBill.second != nullptr)
        showCurrentSphericalBillboard();
}
