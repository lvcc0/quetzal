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

#include "model.h"
#include "rigid_body.h"
#include "billboards.h"
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
    std::pair<std::string, std::shared_ptr<RigidBody>> m_CurrentRigidBody = std::pair<std::string, std::shared_ptr<RigidBody>>("", nullptr);
    std::pair<std::string, std::shared_ptr<CylindricalBillboard>> m_CurrentCylindricalBillboard = std::pair<std::string, std::shared_ptr<CylindricalBillboard>>("", nullptr);
    std::pair<std::string, std::shared_ptr<SphericalBillboard>> m_CurrentSphericalBillboard = std::pair<std::string, std::shared_ptr<SphericalBillboard>>("", nullptr);

private:
    void showCurrentRigidBodyGuiWindow();
    void showCurrentCylBillboard();
    void showCurrentSphericalBillboard();
};