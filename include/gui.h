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

class GUI_Window_object_properties;

// Singleton (should be)
class GUI {
public:
    GUI(GLFWwindow* window);
    GUI(const GUI& obj) = delete;
    GUI(GUI&& obj) = delete;

    // Destructor
    ~GUI();

    // main funcs //
    void guiLoop(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene);

    void showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene);
    void clickWindow(const std::shared_ptr<Scene_Node> obj);
    void clickWindow(const std::shared_ptr<Renderable> obj);
    // ------------------ //

private:
    std::vector<std::shared_ptr<GUI_Window_object_properties>> m_WindowsVec;

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
};

// This class of windows operating with Scene_Node objects
// But constructors get their object via Scene_Node
class GUI_Window_object_properties {
public:
    // Constructors
    GUI_Window_object_properties(const std::shared_ptr<Scene_Node> obj);
    GUI_Window_object_properties(const GUI_Window_object_properties& window) = delete;
    GUI_Window_object_properties(GUI_Window_object_properties&& window) = delete;

    ~GUI_Window_object_properties();

    // Operators
    GUI_Window_object_properties& operator= (const GUI_Window_object_properties&) = delete;

    void windowLoop();

    inline const std::shared_ptr<Scene_Node> getNode() const { return m_SceneNode; }
private:
    std::shared_ptr<Scene_Node> m_SceneNode;

    template <typename T>
    void showCurrentObjectGUI()
    {
        ASSERT(false);
    }
    template<>
    void showCurrentObjectGUI<Model>()
    {
        std::shared_ptr<Model> m_CurrentModel = std::dynamic_pointer_cast<Model>(m_SceneNode);
        m_CurrentModel->is_selected = true;

        ImGui::Begin((m_CurrentModel->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentModel->m_Position, 0.5f);

        ImGui::Separator();
        ImGui::DragFloat3("Scale", (float*)&m_CurrentModel->m_Scale, 0.2f);

        ImGui::Separator();
        ImGui::DragFloat3("Rotation", (float*)&m_CurrentModel->m_RotationDegrees, 1.0f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<RigidBody>()
    {
        std::shared_ptr<RigidBody> m_CurrentRigidBody = std::dynamic_pointer_cast<RigidBody>(m_SceneNode);
        m_CurrentRigidBody->is_selected = true;

        ImGui::Begin((m_CurrentRigidBody->getName() + " config").c_str());

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
        std::shared_ptr<CylindricalBillboard> m_CurrentCylBill = std::dynamic_pointer_cast<CylindricalBillboard>(m_SceneNode);
        m_CurrentCylBill->is_selected = true;

        ImGui::Begin((m_CurrentCylBill->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill->m_Position, 0.5f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<SphericalBillboard>()
    {
        std::shared_ptr<SphericalBillboard> m_CurrentSphBill = std::dynamic_pointer_cast<SphericalBillboard>(m_SceneNode);
        m_CurrentSphBill->is_selected = true;

        ImGui::Begin((m_CurrentSphBill->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill->m_Position, 0.5f);

        ImGui::End();
    }

};