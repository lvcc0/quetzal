#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scene.h"

// Singleton
class GUI {

    GUI(); 
    GUI(const GUI& obj) = delete;
    GUI(GUI&& obj) = delete;

    // Destructor
    ~GUI();

    // Delete operator for copying
    GUI& operator= (GUI const&) = delete;

public:
    static GUI& Instance(GLFWwindow* window); // INIT CRUTCH
    // main funcs //
    void mainGUILoop();
    void showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene);
    // ------------------ //

    // TODO:: Make only one object selectable at time
    std::pair<std::string, std::shared_ptr<Renderable>> m_CurrentRenderable = std::make_pair<std::string, std::shared_ptr<Renderable>>("", nullptr);

private:

    template <typename T>
    void showCurrentObjectGUI() 
    {
        static_assert(false);
    }
    template<>
    void showCurrentObjectGUI<RigidBody>() 
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
    template<>
    void showCurrentObjectGUI<CylindricalBillboard>()
    {
        ImGui::Begin((m_CurrentCylBill.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill.second->m_Position, 0.5f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<SphericalBillboard>()
    {
        ImGui::Begin((m_CurrentSphBill.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill.second->m_Position, 0.5f);

        ImGui::End();
    }

    void cleanCurrents();

    std::pair<std::string, std::shared_ptr<RigidBody>> m_CurrentRigidBody = std::pair<std::string, std::shared_ptr<RigidBody>>("", nullptr);
    std::pair<std::string, std::shared_ptr<Model>> m_CurrentModel = std::pair<std::string, std::shared_ptr<Model>>("", nullptr);
    std::pair<std::string, std::shared_ptr<CylindricalBillboard>> m_CurrentCylBill = std::pair<std::string, std::shared_ptr<CylindricalBillboard>>("", nullptr);
    std::pair<std::string, std::shared_ptr<SphericalBillboard>> m_CurrentSphBill = std::pair<std::string, std::shared_ptr<SphericalBillboard>>("", nullptr);
};