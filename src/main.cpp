#include <iostream>
#include <stb_image/stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "model.h"
#include "billboard.h"
#include "lights.h"
#include "shader.h"
#include "resource_manager.h"
#include "debugger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void showGuiWindow();

const unsigned int WIN_WIDTH = 1280;
const unsigned int WIN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool shouldDrawGui = false;

float FOV = 45.0f;

float materialShininess = 32.0f;

Camera cam(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

std::vector<DirLight> dirLights =
{
    DirLight(
        true,
        "dirLight0",
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    ),
    DirLight(
        true,
        "dirLight1",
        glm::vec3(0.2f, -1.0f, 0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    )
};

std::vector<PointLight> pointLights =
{
    PointLight(
        true,
        "pointLight0",
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(1.0f, 0.0f, 0.0f)
    ),
    PointLight(
        true,
        "pointLight1",
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.0f, 1.0f, 0.0f)
    )
};

std::vector<SpotLight> spotLights =
{
    SpotLight(
        true,
        "flashlight",
        cam.m_pos,
        cam.m_orientation,
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        12.5f,
        15.0f,
        glm::vec3(1.0f, 1.0f, 1.0f)
    ),
    SpotLight(
        true,
        "spotLight0",
        glm::vec3(5.0f, 0.0f, -3.5f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f,
        7.5f,
        10.0f,
        glm::vec3(0.0f, 0.0f, 1.0f)
    )
};

std::vector<Billboard> pointBillboards;
std::vector<Billboard> spotBillboards;

int main()
{
    // --- Initial Config --- //
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "window", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "can't create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    gladLoadGL();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*set stencil shader*/
    glEnable(GL_STENCIL);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    /*Culling faces*/
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // --- //

    ResourceManager resourceManager(RES_PATH);
    Debugger debugger;

    for (unsigned int i = 0; i < pointLights.size(); i++)
        pointBillboards.push_back(Billboard(pointLights[i].m_pos, glm::vec2(1.0f, 1.0f), resourceManager.make_texture("lightbulb", "texture_diffuse", "textures/lightbulb.png")));
    
    for (unsigned int i = 0; i < spotLights.size(); i++)
        spotBillboards.push_back(Billboard(spotLights[i].m_pos, glm::vec2(1.0f, 1.0f), resourceManager.make_texture("highlight", "texture_diffuse", "textures/highlight.png")));

    auto defaultShader = resourceManager.make_shader_program("default_shader", "shaders/default.vert", "shaders/default.frag");

    auto catcube = resourceManager.make_model("catcube", "objects/catcube/catcube.obj");
    auto anothercat = resourceManager.make_model("anothercat", "objects/catcube/catcube.obj");
    auto catsphere = resourceManager.make_model("catsphere", "objects/catsphere/catsphere.obj");

    Billboard billboard(glm::vec3(-3.0f, -3.0f, 0.0f), glm::vec2(5.0f, 5.0f), resourceManager.make_texture("pepe", "none", "textures/pepe.png"));
    
    resourceManager.getObjectsInMaps(ObjectType::TEXTURE);

    // --- Main Loop --- //
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (shouldDrawGui)
            showGuiWindow();

        float curFrame = (float)glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        processInput(window);

        defaultShader->Activate();

        glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.getViewMatrix();

        defaultShader->setVec3("viewPos", cam.m_pos);
        defaultShader->setFloat("material.shininess", materialShininess);

        // --- Rendering Lights' Influence On Objects + Billboards --- //
        for (unsigned int i = 0; i < dirLights.size(); i++)
            dirLights[i].UpdateUni(defaultShader, i);

        for (auto i = 0; i < pointLights.size(); i++)
        {
            pointLights[i].UpdateUni(defaultShader, i);

            if (pointLights[i].m_draw_billboard)
            {
                pointBillboards[i].Draw(defaultShader, cam.getViewMatrix());
                pointBillboards[i].m_pos = pointLights[i].m_pos;
            }
        }

        for (auto i = 0; i < spotLights.size(); i++)
        {
            if (spotLights[i].m_name == "flashlight")
            {
                spotLights[i].m_pos = cam.m_pos;
                spotLights[i].m_dir = cam.m_orientation;
            }

            spotLights[i].UpdateUni(defaultShader, i);

            if (spotLights[i].m_draw_billboard && spotLights[i].m_name != "flashlight")
            {
                spotBillboards[i].Draw(defaultShader, cam.getViewMatrix());
                spotBillboards[i].m_pos = spotLights[i].m_pos;
            }
        }
        // --- //

        defaultShader->setMat4("projection", proj);
        defaultShader->setMat4("view", view);

        catcube->translate(glm::vec3(7.0f, 8.0f, 0.0f));
        anothercat->translate(glm::vec3(-5.0f, 2.0f, -8.0f));
        catsphere->translate(glm::vec3(4.0f, -2.0f, -2.0f));

        billboard.translate(glm::vec3(cos(curFrame) * 3, 0.0f, sin(curFrame) * 3));

        catcube->rotate(curFrame * 10, glm::vec3(0.0f, 1.0f, 0.0f));
        anothercat->scale(glm::vec3(0.5f + sin(curFrame), 0.5f + cos(curFrame), 1.0f));
        catsphere->rotate(curFrame * 50, glm::vec3(0.0f, 1.0f, 0.0f));

        catcube->Draw(defaultShader);
        anothercat->Draw(defaultShader);
        catsphere->Draw(defaultShader);

        billboard.Draw(defaultShader, cam.getViewMatrix());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    // --- //

    // --- Cleaning up --- //
    defaultShader->Delete();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    // --- //

    return 0;
}

// Gets called upon window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cam.UpdateSize(width, height);
}

// Gets called upon key press
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        shouldDrawGui = !shouldDrawGui;

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        spotLights[0].m_enabled = !spotLights[0].m_enabled;
}

// Gets called every frame
void processInput(GLFWwindow* window)
{
    cam.Inputs(window, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

// Shows custom gui window for configuration and stuff
void showGuiWindow()
{
    ImGui::Begin("Stuff");
    
    if (ImGui::CollapsingHeader("Directional Lights"))
    {
        static int dirComboItem = 0;

        if (ImGui::BeginCombo("Source", dirLights[dirComboItem]))
        {
            for (int i = 0; i < dirLights.size(); i++)
            {
                const bool is_selected = (dirComboItem == i);
                if (ImGui::Selectable(dirLights[i], is_selected))
                    dirComboItem = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
        ImGui::Checkbox("Enabled", &dirLights[dirComboItem].m_enabled);
        ImGui::SliderFloat3("Direction", (float*)&dirLights[dirComboItem].m_dir, -1.0f, 1.0f);

        ImGui::Separator();
        ImGui::ColorEdit3("Ambient", (float*)&dirLights[dirComboItem].m_ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&dirLights[dirComboItem].m_diffuse);
        ImGui::ColorEdit3("Specular", (float*)&dirLights[dirComboItem].m_specular);

        ImGui::Separator();
        ImGui::ColorEdit3("Color", (float*)&dirLights[dirComboItem].m_color);
        
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Point Lights"))
    {
        static int pointComboItem = 0;

        if (ImGui::BeginCombo("Source", pointLights[pointComboItem]))
        {
            for (int i = 0; i < pointLights.size(); i++)
            {
                const bool is_selected = (pointComboItem == i);
                if (ImGui::Selectable(pointLights[i], is_selected))
                    pointComboItem = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Enabled", &pointLights[pointComboItem].m_enabled);
        ImGui::Checkbox("Billboard", &pointLights[pointComboItem].m_draw_billboard);
        ImGui::DragFloat3("Position", (float*)&pointLights[pointComboItem].m_pos, 0.1f);

        ImGui::Separator();
        ImGui::ColorEdit3("Ambient", (float*)&pointLights[pointComboItem].m_ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&pointLights[pointComboItem].m_diffuse);
        ImGui::ColorEdit3("Specular", (float*)&pointLights[pointComboItem].m_specular);

        ImGui::Separator();
        ImGui::InputFloat("Constant", (float*)&pointLights[pointComboItem].m_constant, 0.1f, 1.0f);
        ImGui::InputFloat("Linear", (float*)&pointLights[pointComboItem].m_linear, 0.01f, 0.1f);
        ImGui::InputFloat("Quadratic", (float*)&pointLights[pointComboItem].m_quad, 0.001f, 0.01f);

        ImGui::Separator();
        ImGui::ColorEdit3("Color", (float*)&pointLights[pointComboItem].m_color);

        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Spot Lights"))
    {
        static int spotComboItem = 0;

        if (ImGui::BeginCombo("Source", spotLights[spotComboItem]))
        {
            for (int i = 0; i < spotLights.size(); i++)
            {
                const bool is_selected = (spotComboItem == i);
                if (ImGui::Selectable(spotLights[i], is_selected))
                    spotComboItem = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Enabled", &spotLights[spotComboItem].m_enabled);
        ImGui::Checkbox("Billboard", &spotLights[spotComboItem].m_draw_billboard);

        if (spotLights[spotComboItem].m_name != "flashlight")
        {
            ImGui::DragFloat3("Position", (float*)&spotLights[spotComboItem].m_pos, 0.1f);
            ImGui::SliderFloat3("Direction", (float*)&spotLights[spotComboItem].m_dir, -1.0f, 1.0f);
        }

        ImGui::Separator();
        ImGui::ColorEdit3("Ambient", (float*)&spotLights[spotComboItem].m_ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&spotLights[spotComboItem].m_diffuse);
        ImGui::ColorEdit3("Specular", (float*)&spotLights[spotComboItem].m_specular);

        ImGui::Separator();
        ImGui::InputFloat("Constant", (float*)&spotLights[spotComboItem].m_constant, 0.1f, 1.0f);
        ImGui::InputFloat("Linear", (float*)&spotLights[spotComboItem].m_linear, 0.01f, 0.1f);
        ImGui::InputFloat("Quadratic", (float*)&spotLights[spotComboItem].m_quad, 0.001f, 0.01f);

        ImGui::Separator();
        ImGui::DragFloat("Inner Angle", (float*)&spotLights[spotComboItem].m_innerCutoff, 0.5f, 0.0f, 90.0f);
        ImGui::DragFloat("Outer Angle", (float*)&spotLights[spotComboItem].m_outerCutoff, 0.5f, 0.0f, 90.0f);

        ImGui::Separator();
        ImGui::ColorEdit3("Color", (float*)&spotLights[spotComboItem].m_color);

        ImGui::Separator();
    }

    ImGui::SeparatorText("Material");
    ImGui::SliderFloat("Material Shininess", (float*)&materialShininess, 1.0f, 256.0f);

    ImGui::SeparatorText("Screen");
    ImGui::SliderFloat("FOV", &FOV, 0.0f, 120.0f);

    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", cam.m_pos.x, cam.m_pos.y, cam.m_pos.z);
    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", cam.m_orientation.x, cam.m_orientation.y, cam.m_orientation.z);

    ImGui::Text("%.3f ms (%.1f FPS)", deltaTime * 1000.0f, 1.0f / deltaTime);

    ImGui::End();
}