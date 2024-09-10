#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <typeinfo>

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

    // One selectable object
    std::pair<std::string, std::shared_ptr<Renderable>> m_CurrentRenderable = std::make_pair<std::string, std::shared_ptr<Renderable>>("", nullptr);

private:
    template<typename T>
    void addObject(std::string name, std::shared_ptr<Scene> scene)
    {
        ASSERT(false);
    }
    template<>
    void addObject<Model>(std::string name, std::shared_ptr<Scene> scene)
    {
        scene->addModel(name);
    }

    template <typename T>
    void showCurrentObjectGUI() 
    {
        ASSERT(false);
    }
    template<>
    void showCurrentObjectGUI<Model>()
    {
        std::shared_ptr<Model> m_CurrentModelBody = std::static_pointer_cast<Model>(m_CurrentRenderable.second);

        ImGui::Begin((m_CurrentRenderable.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentModelBody->m_Position, 0.5f);

        ImGui::Separator();
        ImGui::DragFloat3("Scale", (float*)&m_CurrentModelBody->m_Scale, 0.2f);

        ImGui::Separator();
        ImGui::DragFloat3("Rotation", (float*)&m_CurrentModelBody->m_RotationDegrees, 1.0f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<RigidBody>() 
    {
        std::shared_ptr<RigidBody> m_CurrentRigidBody = std::static_pointer_cast<RigidBody>(m_CurrentRenderable.second);

        ImGui::Begin((m_CurrentRenderable.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentRigidBody->m_Position, 0.5f);

        ImGui::Separator();
        ImGui::DragFloat3("Move speed", (float*)&m_CurrentRigidBody->m_MoveVector, 0.001f);

        ImGui::Separator();
        ImGui::DragFloat3("Scale", (float*)&m_CurrentRigidBody->m_Scale, 0.2f);

        ImGui::Separator();
        ImGui::DragFloat3("Rotation", (float*)&m_CurrentRigidBody->m_RotationDegrees, 1.0f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<CylindricalBillboard>()
    {
        std::shared_ptr<CylindricalBillboard> m_CurrentCylBill = std::static_pointer_cast<CylindricalBillboard>(m_CurrentRenderable.second);

        ImGui::Begin((m_CurrentRenderable.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill->m_Position, 0.5f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<SphericalBillboard>()
    {
        std::shared_ptr<SphericalBillboard> m_CurrentSphBill = std::static_pointer_cast<SphericalBillboard>(m_CurrentRenderable.second);

        ImGui::Begin((m_CurrentRenderable.first + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill->m_Position, 0.5f);

        ImGui::End();
    }
};