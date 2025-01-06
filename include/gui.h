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

#include "scene.h"

// NOTE: USE IMGUI WINDOW STATE FUNCS BETWEEN CALLING IMGUI::RENDER AND IMGUI::END 

template <class T>
concept constraint_window_properties = std::is_same_v<T, glm::vec2> || std::is_same_v<T, std::vector<glm::vec2>>;

// You must prepare your inheritor for recieving info about some parameters
template<class T> requires constraint_window_properties<T>
struct WindowProperties
{
public:
    inline T getGUIWindowSize() const noexcept { return GUIWindowSize; };
    inline T getGUIWindowPos() const noexcept { return GUIWindowPos; };

protected:
    unsigned int pos_x, pos_y, width, height;

    // These particular params must be taken between ImGui::Render and ImGui::End
    // So we somehow get them and keep here
    inline static T GUIWindowSize;
    inline static T GUIWindowPos;

    static void positioningWindow(GLFWwindow* window, float part_of_width, float part_of_height, float pos_x, float pos_y);
};

// This class of windows operating with Scene_Node objects
// But constructors get their object via Scene_Node
class GUIWindow : public WindowProperties<std::vector<glm::vec2>>
{
public:
    GUIWindow(const std::shared_ptr<qtzl::Node> obj, const std::vector<std::shared_ptr<qtzl::Node>> scene_nodes);
    GUIWindow(const GUIWindow&) = delete;
    GUIWindow(GUIWindow&&) = delete;

    ~GUIWindow();

    GUIWindow& operator= (const GUIWindow&) = delete;

    void windowLoop();

    inline const std::shared_ptr<qtzl::Node> getNode() const { return m_SceneNode; }

private:
    std::shared_ptr<qtzl::Node> m_SceneNode;
    std::vector<std::shared_ptr<qtzl::Node>> m_AllSceneNodes;
    bool attaching{ false };

    template <typename T>
    void showCurrentObjectGUI();
    template<>
    void showCurrentObjectGUI<Model>();
    template<>
    void showCurrentObjectGUI<CylindricalBillboard>();
    template<>
    void showCurrentObjectGUI<SphericalBillboard>();
};

// This is the main part of GUI of scenes. It contains more than one window
class GUI : public WindowProperties<std::vector<glm::vec2>>
{
public:
    GUI() = delete;
    GUI(const GUI&) = delete;
    GUI(GUI&&) = delete;

    static void init(GLFWwindow* window);
    static void shutdown();

    static void render(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window);

    static void showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window);
    static void createWindow(const std::shared_ptr<qtzl::Node> obj);
    static void createWindow(const std::shared_ptr<qtzl::Node3D> obj);

    inline static std::vector<std::shared_ptr<GUIWindow>> getWindows() { return m_Windows; }

private:
    inline static std::vector<std::shared_ptr<GUIWindow>> m_Windows;

    //template<typename T>
    //void addObject(std::string name, std::shared_ptr<Scene> scene)
    //{
    //    ASSERT(false);
    //}
    //template<>
    //void addObject<Model>(std::string name, std::shared_ptr<Scene> scene)
    //{
    //    scene->addModel(name);
    //}
};