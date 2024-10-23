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

class GUI_Window_object_properties {
public:
    // Constructors
    GUI_Window_object_properties(const std::shared_ptr<Renderable> obj);
    GUI_Window_object_properties(const GUI_Window_object_properties& window) = delete;
    GUI_Window_object_properties(GUI_Window_object_properties&& window) = delete;

    ~GUI_Window_object_properties();

    // Operators
    GUI_Window_object_properties& operator= (const GUI_Window_object_properties&) = delete;

    void windowLoop();

    inline const std::shared_ptr<Renderable> getRenderable() const { return m_Renderable; }
private:
    std::shared_ptr<Renderable> m_Renderable;

    template <typename T>
    void showCurrentObjectGUI(std::shared_ptr<Renderable> obj)
    {
        ASSERT(false);
    }
    template<>
    void showCurrentObjectGUI<Model>(std::shared_ptr<Renderable> obj)
    {
        std::shared_ptr<Model> m_CurrentModel = std::static_pointer_cast<Model>(obj);

        ImGui::Begin((obj->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentModel->m_Position, 0.5f);

        ImGui::Separator();
        ImGui::DragFloat3("Scale", (float*)&m_CurrentModel->m_Scale, 0.2f);

        ImGui::Separator();
        ImGui::DragFloat3("Rotation", (float*)&m_CurrentModel->m_RotationDegrees, 1.0f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<RigidBody>(std::shared_ptr<Renderable> obj)
    {
        std::shared_ptr<RigidBody> m_CurrentRigidBody = std::static_pointer_cast<RigidBody>(obj);

        ImGui::Begin((obj->getName() + " config").c_str());

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
    void showCurrentObjectGUI<CylindricalBillboard>(std::shared_ptr<Renderable> obj)
    {
        std::shared_ptr<CylindricalBillboard> m_CurrentCylBill = std::static_pointer_cast<CylindricalBillboard>(obj);

        ImGui::Begin((obj->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentCylBill->m_Position, 0.5f);

        ImGui::End();
    }
    template<>
    void showCurrentObjectGUI<SphericalBillboard>(std::shared_ptr<Renderable> obj)
    {
        std::shared_ptr<SphericalBillboard> m_CurrentSphBill = std::static_pointer_cast<SphericalBillboard>(obj);

        ImGui::Begin((obj->getName() + " config").c_str());

        ImGui::Separator();
        ImGui::DragFloat3("Position", (float*)&m_CurrentSphBill->m_Position, 0.5f);

        ImGui::End();
    }
};