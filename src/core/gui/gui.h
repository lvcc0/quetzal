#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <concepts>
#include <type_traits>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/scene.h"

#include "core/gui/engine_window.h"
#include "core/gui/node_window.h"

// This is the main part of scene GUI. It contains more than one window
class GUI
{
public:
    GUI() = delete;
    GUI(const GUI&) = delete;
    GUI(GUI&&) = delete;

    static void init(GLFWwindow* window);
    static void shutdown();

    static void render(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time);

    static void showSceneConfig(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time);
    static void showNodeManager(std::shared_ptr<Scene> scene);
    static void showResourceManager();

    // Creating or destroying windows for nodes
    static void onClick(const std::shared_ptr<qtzl::Node3D> node);
    
    // Check if the given position is occupied by any gui window
    static bool isOccupied(double x, double y);

    inline static std::vector<std::shared_ptr<qtzl::NodeWindow>> getNodeWindows();

private:
    inline static std::vector<std::shared_ptr<qtzl::NodeWindow>> m_NodeWindows;
    inline static std::vector<std::shared_ptr<qtzl::EngineWindow>> m_EngineWindows;
};