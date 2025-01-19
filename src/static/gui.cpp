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

void GUI::render(std::string& scene_name, const std::map<const std::string, std::shared_ptr<Scene>>& scenes, GLfloat delta_time)
{
    showSceneConfig(scene_name, scenes, delta_time); // scene config is always visible if GUI is enabled

    if (m_NodeMgrVisible)
        showNodeManager(scene_name, scenes.at(scene_name));
    if (m_ResourceMgrVisible)
        showResourceManager();
}

void GUI::showSceneConfig(std::string& scene_name, const std::map<const std::string, std::shared_ptr<Scene>>& scenes, GLfloat delta_time)
{
    ImGui::Begin((scene_name + " config").c_str(), 0);

    if (!TEXT_BASE_WIDTH)
        TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

    ImGui::Checkbox("Show node manager", &m_NodeMgrVisible);
    ImGui::Checkbox("Show resource manager", &m_ResourceMgrVisible);

    ImGui::Separator();

    ImGui::Checkbox("Physics Enabled", &scenes.at(scene_name)->m_IsPhysicsProcessing);

    if (ImGui::Checkbox("Postprocessing Enabled", &scenes.at(scene_name)->m_IsPostProcessing))
        glEnable(GL_DEPTH_TEST);

    // TODO: also do this as a table?
    if (scenes.at(scene_name)->m_IsPostProcessing)
    {
        ImGui::Separator();

        std::vector<std::string> enabledShaders = scenes.at(scene_name)->m_PostProcessing.getEnabledPPShaders(); // current screen shader names

        for (const auto& name : scenes.at(scene_name)->m_PostProcessing.getPPShaderNames())
        {
            auto it = std::find(enabledShaders.begin(), enabledShaders.end(), name);

            if (ImGui::Selectable(name.substr(name.find("||") + 2).c_str(), it != enabledShaders.end()))
            {
                scenes.at(scene_name)->m_PostProcessing.setPPShaderEnabled(name, it == enabledShaders.end());
                glEnable(GL_DEPTH_TEST); // postprocessing disables depth test after as it's final step, so we need to turn it back on
            }
            ImGui::SameLine(ImGui::GetWindowSize().x - TEXT_BASE_WIDTH * 12.0f); ImGui::Text((it != enabledShaders.end()) ? "enabled" : "disabled");
        }

        ImGui::Separator();
    }

    // TODO: tools to create nodes on the fly

    ImGui::SeparatorText("Scenes");

    if (ImGui::BeginListBox("##scenes", ImVec2(-FLT_MIN, std::min(5, (int)scenes.size()) * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (const auto& scene : scenes)
        {
            const bool isSelected = (scene.first == scene_name);

            if (ImGui::Selectable(scene.first.c_str(), isSelected))
            {
                scene_name = scene.first;
                m_CurrentNode_sptr.reset();
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

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
    ImGui::DragFloat("Cam speed", &scenes.at(scene_name)->m_Camera.m_Speed, 0.1f, 0.1f, 1000.0f, "%.1f");
    ImGui::DragFloat("Cam sens", &scenes.at(scene_name)->m_Camera.m_Sensitivity, 0.1f, 0.1f, 1000.0f, "%.2f");

    ImGui::Separator();

    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", scenes.at(scene_name)->m_Camera.m_Position.x, scenes.at(scene_name)->m_Camera.m_Position.y, scenes.at(scene_name)->m_Camera.m_Position.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", scenes.at(scene_name)->m_Camera.m_Orientation.x, scenes.at(scene_name)->m_Camera.m_Orientation.y, scenes.at(scene_name)->m_Camera.m_Orientation.z);
    ImGui::Text("Framebuffer size: %ux%u", m_FramebufferWidth, m_FramebufferHeight);
    ImGui::Text("%.3f ms (%.1f FPS)", delta_time * 1000.0f, 1.0f / delta_time);

    ImGui::End();
}

void GUI::showNodeManager(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
    ImGui::Begin((scene_name + " node mgr").c_str(), 0);

    ImGui::Checkbox("Show node types", &m_NodeMgrShowType);

    // Scene nodes list
    if (ImGui::BeginTable("Nodes", 1 + (int)m_NodeMgrShowType, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
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

    // Selected node config
    if (m_CurrentNode_sptr != nullptr)
    {
        ImGui::SeparatorText(m_CurrentNode_sptr->getString("Name").c_str());

        // Int properties
        for (auto& entry : m_CurrentNode_sptr->getIntProperties())
        {
            ImGui::InputInt(entry.first.c_str(), &entry.second);

            if (ImGui::IsItemDeactivatedAfterEdit())
                m_CurrentNode_sptr->set(entry.first, entry.second);
        }

        // Float properties
        for (auto& entry : m_CurrentNode_sptr->getFloatProperties())
        {
            ImGui::InputFloat(entry.first.c_str(), &entry.second);

            if (ImGui::IsItemDeactivatedAfterEdit())
                m_CurrentNode_sptr->set(entry.first, entry.second);
        }

        // Bool properties
        for (auto& entry : m_CurrentNode_sptr->getBoolProperties())
        {
            ImGui::Checkbox(entry.first.c_str(), &entry.second);

            if (ImGui::IsItemDeactivatedAfterEdit())
                m_CurrentNode_sptr->set(entry.first, entry.second);
        }

        // String properties
        for (auto& entry : m_CurrentNode_sptr->getStringProperties())
        {
            char* text = (char*)entry.second.c_str();

            ImGui::InputText(entry.first.c_str(), text, 32);

            if (ImGui::IsItemDeactivatedAfterEdit())
                m_CurrentNode_sptr->set(entry.first, (std::string)text);
        }

        // Vec3 properties
        for (auto& entry : m_CurrentNode_sptr->getVec3Properties())
        {
            float* data = glm::value_ptr(entry.second);

            if (ImGui::DragFloat3(entry.first.c_str(), data, 0.1f))
                m_CurrentNode_sptr->set(entry.first, glm::make_vec3(data));
        }
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
        bool open = ImGui::TreeNodeEx(node->getString("Name").c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick);
        
        if (ImGui::IsItemClicked())
            m_CurrentNode_sptr = node;

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
        ImGui::TreeNodeEx(node->getString("Name").c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

        if (ImGui::IsItemClicked())
            m_CurrentNode_sptr = node;

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