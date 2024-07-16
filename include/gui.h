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
public:
    GUI(); // TODO:: CHANGE INITIALIZATION
    GUI(const GUI& obj) = delete;
    GUI(GUI&& obj) = delete;

    void initialize(GLFWwindow* window); // INIT CRUTCH
    // main funcs //
    void mainGUILoop();
    void showCurrentSceneGUI(GLfloat delta_time);
    // ------------------ //

    void destroyGUI();

    std::map<const std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene = "";

    std::pair<std::string, std::shared_ptr<RigidBody>> m_CurrentRigidBody = std::pair<std::string, std::shared_ptr<RigidBody>>("", nullptr);
    std::pair<std::string, std::shared_ptr<CylindricalBillboard>> m_CurrentCylindricalBillboard = std::pair<std::string, std::shared_ptr<CylindricalBillboard>>("", nullptr);
    std::pair<std::string, std::shared_ptr<SphericalBillboard>> m_CurrentSphericalBillboard = std::pair<std::string, std::shared_ptr<SphericalBillboard>>("", nullptr);

private:
    void showCurrentRigidBodyGuiWindow();
    void showCurrentCylBillboard();
    void showCurrentSphericalBillboard();
};