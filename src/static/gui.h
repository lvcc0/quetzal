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

#include "static/renderer.h"
#include "core/scene.h"

// TODO: show actual loaded resource in the resource manager gui

// This is the main part of scene GUI. It contains more than one window
class GUI
{
public:
    GUI() = delete;
    GUI(const GUI&) = delete;
    GUI(GUI&&) = delete;

    static void init(GLFWwindow* window);
    static void shutdown();

    static void render(std::string& scene_name, const std::map<std::string, std::shared_ptr<Scene>>& scenes, GLfloat delta_time);

    static void showSceneConfig(std::string& scene_name, const std::map<std::string, std::shared_ptr<Scene>>& scenes, GLfloat delta_time);
    static void showNodeManager(const std::string& scene_name, std::shared_ptr<Scene> scene);
    static void showResourceManager();

    // TODO: repurpose this stuff
    static void onClick(const std::shared_ptr<qtzl::Node3D> node);
    
    // Check if the given position is occupied by any gui window
    static bool isOccupied(double x, double y);

    // Calls upon Engine::framebufferSizeCallback
    static void updateFramebufferSize(int width, int height);

private:
    inline static float TEXT_BASE_WIDTH;

    inline static bool m_NodeMgrVisible = true;
    inline static bool m_ResourceMgrVisible = true;

    inline static bool m_VSyncCur = true;
    inline static bool m_VSyncOld = m_VSyncCur;

    inline static int m_FramebufferWidth;
    inline static int m_FramebufferHeight;

    inline static std::shared_ptr<qtzl::Node> m_CurrentNode_sptr;

    inline static bool m_NodeMgrShowType = false;

    // Used to fill the node tree in the node manager window
    static void displayNode(std::shared_ptr<qtzl::Node> node);

    // Used to fill the file tree in the resource manager window
    static void displayDirEntry(std::filesystem::directory_entry entry);
};