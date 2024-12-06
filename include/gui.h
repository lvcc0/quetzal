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


// TIP: USE IMGUI WINDOW STATE FUNCS BETWEEN CALLING IMGUI::RENDER AND IMGUI::END 

class GUI_Window_object_properties; // GUI must be able to know about this class

template <class T>
concept constraint_window_properties = std::is_same_v<T, glm::vec2> || std::is_same_v <T, std::vector<glm::vec2>>;

// You must prepare your inheritor for recieving info about some parameters
template<class Type> requires constraint_window_properties<Type>
struct WindowProperties
{
protected:
    unsigned int pos_x, pos_y, width, height;
    // These particular params must be taken between ImGui::Render and ImGui::End
    // So we somehow get them and keep here
    Type GUIWindowSize;
    Type GUIWindowPos;
    // ----------------- //
    void positioningWindow(GLFWwindow* window, float part_of_width, float part_of_height, float pos_x, float pos_y);
public:
    // getters //
    inline Type getGUIWindowSize() const noexcept { return GUIWindowSize; };
    inline Type getGUIWindowPos() const noexcept { return GUIWindowPos; };
    // -------------------//
};

// Singleton (should be)
// This is the main part of GUI of scenes. It contains more than one window
class GUI: public WindowProperties<std::vector<glm::vec2>>
{
public:
    GUI(GLFWwindow* window);
    GUI(const GUI& obj) = delete;
    GUI(GUI&& obj) = delete;

    // Destructor
    ~GUI();

    // main funcs //
    void guiLoop(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window);

    void showCurrentSceneGUI(GLfloat delta_time, std::pair<std::string, std::shared_ptr<Scene>> current_scene, GLFWwindow* window);
    void clickWindow(const std::shared_ptr<Scene_Node> obj);
    void clickWindow(const std::shared_ptr<Renderable> obj);
    // ------------------ //
    inline std::vector<std::shared_ptr<GUI_Window_object_properties>> getWindowsObjProps()const { return m_WindowsVec; }

private:
    std::vector<std::shared_ptr<GUI_Window_object_properties>> m_WindowsVec;
    std::vector<std::shared_ptr<Scene_Node>> m_AllSceneNodes;

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
class GUI_Window_object_properties : public WindowProperties <std::vector<glm::vec2>>
{
public:
    // Constructors
    GUI_Window_object_properties(const std::shared_ptr<Scene_Node> obj, const std::vector<std::shared_ptr<Scene_Node>> scene_nodes);
    GUI_Window_object_properties(const GUI_Window_object_properties& window) = delete;
    GUI_Window_object_properties(GUI_Window_object_properties&& window) = delete;

    ~GUI_Window_object_properties();

    // Operators
    GUI_Window_object_properties& operator= (const GUI_Window_object_properties&) = delete;

    void windowLoop();

    inline const std::shared_ptr<Scene_Node> getNode() const { return m_SceneNode; }
private:
    std::shared_ptr<Scene_Node> m_SceneNode;
    std::vector<std::shared_ptr<Scene_Node>> m_AllSceneNodes;
    bool attaching{false};

    template <typename T>
    void showCurrentObjectGUI();
    template<>
    void showCurrentObjectGUI<Model>();
    template<>
    void showCurrentObjectGUI<RigidBody>();
    template<>
    void showCurrentObjectGUI<CylindricalBillboard>();
    template<>
    void showCurrentObjectGUI<SphericalBillboard>();
};

