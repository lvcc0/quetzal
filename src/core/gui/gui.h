#pragma once

// std
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

// thirdparty
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
    static void showNodeManager(const std::string& scene_name, std::shared_ptr<Scene> scene);
    static void showResourceManager();

    // Creating or destroying windows for nodes
    static void onClick(const std::shared_ptr<qtzl::Node3D> node);
    
    // Check if the given position is occupied by any gui window
    static bool isOccupied(double x, double y);

    // Calls upon Engine::framebufferSizeCallback
    static void updateFramebufferSize(int width, int height);

    static std::vector<std::shared_ptr<qtzl::NodeWindow>> getNodeWindows();

private:
    inline static bool m_EngineVSyncCur = true;
    inline static bool m_EngineVSyncOld = m_EngineVSyncCur;

    inline static int m_EngineFramebufferWidth;
    inline static int m_EngineFramebufferHeight;

    inline static std::string m_EngineCurrentNodeName;

    inline static bool m_NodeMgrShowClassNames = false;

    inline static std::vector<std::shared_ptr<qtzl::NodeWindow>> m_NodeWindows;
    inline static std::vector<std::shared_ptr<qtzl::EngineWindow>> m_EngineWindows;
};