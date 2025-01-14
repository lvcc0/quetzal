#include "static/gui.h"

void GUI::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    glfwGetFramebufferSize(window, &m_FramebufferWidth, &m_FramebufferHeight);
}

void GUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::render(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time)
{
    showSceneConfig(scene_name, scene, delta_time); // scene config is always visible

    if (m_NodeMgrVisible)
        showNodeManager(scene_name, scene);
    if (m_ResourceMgrVisible)
        showResourceManager();
}

void GUI::showSceneConfig(const std::string& scene_name, std::shared_ptr<Scene> scene, GLfloat delta_time)
{
    ImGui::Begin((scene_name + " config").c_str(), 0);

    if (!TEXT_BASE_WIDTH)
        TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

    ImGui::Checkbox("Show node manager", &m_NodeMgrVisible);
    ImGui::Checkbox("Show resource manager", &m_ResourceMgrVisible);

    ImGui::Separator();

    ImGui::Checkbox("Physics Enabled", &scene->m_IsPhysicsProcessing);

    if (ImGui::Checkbox("Postprocessing Enabled", &scene->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    // TODO: also do this as a table?
    if (scene->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> enabledShaders = scene->m_PostProcessing.getEnabledPPShaders(); // current screen shader names

        for (const auto& name : scene->m_PostProcessing.getPPShaderNames())
        {
            auto it = std::find(enabledShaders.begin(), enabledShaders.end(), name);

            if (ImGui::Selectable(name.substr(name.find("||") + 2).c_str(), it != enabledShaders.end()))
            {
                scene->m_PostProcessing.setPPShaderEnabled(name, it == enabledShaders.end());
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - TEXT_BASE_WIDTH * 12.0f); ImGui::Text((it != enabledShaders.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    // TODO: tools to create nodes on the fly

    ImGui::SeparatorText("Engine");

    // VSync
    ImGui::Checkbox("VSync Enabled", &m_VSyncCur);
    if (m_VSyncCur != m_VSyncOld)
    {
        m_VSyncOld = m_VSyncCur;
        glfwSwapInterval(m_VSyncCur);
    }

    // Camera config
    ImGui::DragFloat("Cam FOV", &Renderer::m_FOV, 0.5f, 30.0f, 110.0f, "%.1f");
    ImGui::DragFloat("Cam speed", &scene->m_Camera.m_Speed, 0.1f, 0.1f, 1000.0f, "%.1f");
    ImGui::DragFloat("Cam sens", &scene->m_Camera.m_Sensitivity, 0.1f, 0.1f, 1000.0f, "%.2f");

    ImGui::Separator();

    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_Position.x, scene->m_Camera.m_Position.y, scene->m_Camera.m_Position.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", scene->m_Camera.m_Orientation.x, scene->m_Camera.m_Orientation.y, scene->m_Camera.m_Orientation.z);
    ImGui::Text("Framebuffer size: %ux%u", m_FramebufferWidth, m_FramebufferHeight);
    ImGui::Text("%.3f ms (%.1f FPS)", delta_time * 1000.0f, 1.0f / delta_time);

    ImGui::End();
}

void GUI::showNodeManager(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
    ImGui::Begin((scene_name + " node mgr").c_str(), 0);

    ImGui::Checkbox("Show node types", &m_NodeMgrShowType);

    // Scene nodes list
    if (ImGui::BeginTable("Nodes", m_NodeMgrShowType ? 2 : 1, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        if (m_NodeMgrShowType) ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 20.0f);
        ImGui::TableHeadersRow();

        for (const auto& node : scene->getNodes())
        {
            if (node->getParent() == nullptr)
                displayNode(node);
        }

        ImGui::EndTable();
    }

    // TODO: selected node

    // Selected node config
    if (m_CurrentNode_sptr != nullptr)
    {
        ImGui::SeparatorText(m_CurrentNode_sptr->getName().c_str());

        // TODO: do smth like "std::map<std::string, std::any/std::optional> Node::getData()" to configure here
    }

    ImGui::End();
}

void GUI::showResourceManager()
{
    ImGui::Begin("Resource manager", 0);

    // TODO: tools to load resources on the fly
    // TODO: update files button
    // TODO: get full path upon right click or smth

    if (ImGui::BeginTable("Resources", 3, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 13.0f);
        ImGui::TableSetupColumn("Loaded", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 7.0f);
        ImGui::TableHeadersRow();

        displayDirEntry(std::filesystem::directory_entry(RES_PATH));

        ImGui::EndTable();
    }

    ImGui::End();
}

// Making window using node3D
void GUI::onClick(const std::shared_ptr<qtzl::Node3D> node)
{
    // :)
}

bool GUI::isOccupied(double x, double y)
{
    // TODO: there is some imgui property telling if the mouse is on the window or not

    return false;
}

void GUI::updateFramebufferSize(int width, int height)
{
    m_FramebufferWidth = width;
    m_FramebufferHeight = height;
}

void GUI::displayNode(std::shared_ptr<qtzl::Node> node)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (node->getChildren().size() > 0)
    {
        bool open = ImGui::TreeNodeEx(node->getName().c_str(), ImGuiTreeNodeFlags_SpanFullWidth);

        if (m_NodeMgrShowType)
        {
            std::string className = typeid(*node).name();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(className.erase(0, className.find_last_of("::") + 1).c_str());
        }

        if (open)
        {
            for (const auto& entry : node->getChildren())
            {
                displayNode(entry.second);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TreeNodeEx(node->getName().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

        if (m_NodeMgrShowType)
        {
            std::string className = typeid(*node).name();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(className.erase(0, className.find_last_of("::") + 1).c_str());
        }
    }
}

void GUI::displayDirEntry(std::filesystem::directory_entry entry)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (entry.is_directory())
    {
        std::string filename = entry.path().filename().string();

        bool open = ImGui::TreeNodeEx((!filename.empty()) ? filename.c_str() : entry.path().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Folder");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("--");

        if (open)
        {
            for (const auto& child_entry : std::filesystem::directory_iterator(entry))
            {
                displayDirEntry(child_entry);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TreeNodeEx(entry.path().filename().string().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(ResourceManager::getType(entry.path().string()).c_str());
        ImGui::TableNextColumn();
        ImGui::TextUnformatted((ResourceManager::isLoaded(entry.path().string())) ? "Yes" : "No");
    }
}