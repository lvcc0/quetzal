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

enum class EngineWindowType { SceneCfg, NodeMgr, ResourceMgr };

// Base window class
class GUIWindow
{
public:
    GUIWindow();
    GUIWindow(const GUIWindow&) = delete;
    GUIWindow(GUIWindow&&) = delete;

    ~GUIWindow();

    GUIWindow& operator= (const GUIWindow&) = delete;

    inline glm::vec2 getPosition() const { return m_Position; }
    inline glm::vec2 getSize() const { return m_Size; }

protected:
    glm::vec2 m_Position;
    glm::vec2 m_Size;
};

// NOTE: maybe we'll add some more stuff to this class
class GUIEngineWindow : public GUIWindow
{
public:
    GUIEngineWindow(EngineWindowType type);

    EngineWindowType m_Type;

    bool isVisible() const;
    void toggleVisibility();

private:
    bool m_IsVisible = false;
};

// Windows created for nodes only
class GUINodeWindow : public GUIWindow
{
public:
    GUINodeWindow(std::shared_ptr<qtzl::Node3D> node);
    ~GUINodeWindow();

    std::shared_ptr<qtzl::Node3D> getNode() const;

    void render();

private:
    std::shared_ptr<qtzl::Node3D> m_Node;
    
    // TODO: yeah
    //template <typename T> void showNodeGUI();
    //template <typename T> void showNodeGUI</*node*/>();
    //template <typename T> void showNodeGUI</*node*/>();
};

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

    inline static std::vector<std::shared_ptr<GUINodeWindow>> getNodeWindows();

private:
    inline static std::vector<std::shared_ptr<GUINodeWindow>> m_NodeWindows;
    inline static std::vector<std::shared_ptr<GUIEngineWindow>> m_EngineWindows;
};