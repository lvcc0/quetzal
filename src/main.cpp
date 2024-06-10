#include <iostream>
#include <stb_image/stb_image.h>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

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
#include "post_processing.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
//void showGuiWindow();

const unsigned int WIN_WIDTH = 1280;
const unsigned int WIN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool shouldDrawGui = false;

float FOV = 45.0f;

float materialShininess = 32.0f;

Camera cam(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

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

//test code
float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

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

    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //ImGui_ImplOpenGL3_Init();

    stbi_set_flip_vertically_on_load(true);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // idk how to summarize this
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Stencil shader
    //glEnable(GL_STENCIL);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    ////Culling faces
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    // --- //

    ResourceManager resourceManager(RES_PATH);
    Debugger debugger;

    /*for (unsigned int i = 0; i < pointLights.size(); i++)
        pointBillboards.push_back(Billboard(pointLights[i].m_pos, glm::vec2(1.0f, 1.0f), resourceManager.make_texture("lightbulb", "texture_diffuse", "textures/lightbulb.png")));
    
    for (unsigned int i = 0; i < spotLights.size(); i++)
        spotBillboards.push_back(Billboard(spotLights[i].m_pos, glm::vec2(1.0f, 1.0f), resourceManager.make_texture("highlight", "texture_diffuse", "textures/highlight.png")));*/

    //auto defaultShader = resourceManager.make_shader_program("default_shader", "shaders/default.vert", "shaders/default.frag");
    auto testShader = resourceManager.make_shader_program("test_shader", "shaders/test.vert", "shaders/test.frag");
    auto screenShader = resourceManager.make_shader_program("inversion_shader", "shaders/post_processing.vert", "shaders/inversion_color.frag");

    //auto catcube = resourceManager.make_model("catcube", "objects/catcube/catcube.obj");
    //auto anothercat = resourceManager.make_model("anothercat", "objects/catcube/catcube.obj");
    //auto catsphere = resourceManager.make_model("catsphere", "objects/catsphere/catsphere.obj");
    
    GLuint frameVAO;
    GLuint frameBuffer, textureFrameBuffer, renderFrameBuffer;
    
    PostProcessing::make_vertexArray(frameVAO);
    PostProcessing::make_buffers(frameBuffer, textureFrameBuffer, renderFrameBuffer, WIN_WIDTH, WIN_HEIGHT);

    //test code
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    testShader->Activate();
    testShader->setInt("texture1", 0);

    screenShader->Activate();
    screenShader->setInt("screenTexture", 0);

    auto cubeTexture = resourceManager.make_texture("test", "none", "textures/container.png");
    resourceManager.getObjectsInMaps(ObjectType::SHADER);
    // --- Main Loop --- //
    while (!glfwWindowShouldClose(window))
    {

 /*       ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();*/

        //if (shouldDrawGui)
        //    showGuiWindow();

        float curFrame = (float)glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;

        // input 
        processInput(window);

        // Bind the custom framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        // Specify the color of the background
        glClearColor(0.77f, 0.73f, 0.77f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Enable depth testing since it's disabled when drawing the framebuffer rectangle
        glEnable(GL_DEPTH_TEST);
        

        testShader->Activate();
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)cam.m_width / (float)cam.m_height, 0.1f, 100.0f);
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        testShader->setMat4("view", view);
        testShader->setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture->ID);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        testShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        testShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader->Activate();
        glBindVertexArray(frameVAO);
        glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // --- //

    // --- Cleaning up --- //
    //defaultShader->Delete();
    testShader->Delete();
    screenShader->Delete();
    glDeleteBuffers(1, &frameBuffer);

    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

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
//void showGuiWindow()
//{
//    ImGui::Begin("Stuff");
//    
//    if (ImGui::CollapsingHeader("Directional Lights"))
//    {
//        static int dirComboItem = 0;
//
//        if (ImGui::BeginCombo("Source", dirLights[dirComboItem]))
//        {
//            for (int i = 0; i < dirLights.size(); i++)
//            {
//                const bool is_selected = (dirComboItem == i);
//                if (ImGui::Selectable(dirLights[i], is_selected))
//                    dirComboItem = i;
//
//                if (is_selected)
//                    ImGui::SetItemDefaultFocus();
//            }
//            ImGui::EndCombo();
//        }
//        
//        ImGui::Checkbox("Enabled", &dirLights[dirComboItem].m_enabled);
//        ImGui::SliderFloat3("Direction", (float*)&dirLights[dirComboItem].m_dir, -1.0f, 1.0f);
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Ambient", (float*)&dirLights[dirComboItem].m_ambient);
//        ImGui::ColorEdit3("Diffuse", (float*)&dirLights[dirComboItem].m_diffuse);
//        ImGui::ColorEdit3("Specular", (float*)&dirLights[dirComboItem].m_specular);
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Color", (float*)&dirLights[dirComboItem].m_color);
//        
//        ImGui::Separator();
//    }
//
//    if (ImGui::CollapsingHeader("Point Lights"))
//    {
//        static int pointComboItem = 0;
//
//        if (ImGui::BeginCombo("Source", pointLights[pointComboItem]))
//        {
//            for (int i = 0; i < pointLights.size(); i++)
//            {
//                const bool is_selected = (pointComboItem == i);
//                if (ImGui::Selectable(pointLights[i], is_selected))
//                    pointComboItem = i;
//
//                if (is_selected)
//                    ImGui::SetItemDefaultFocus();
//            }
//            ImGui::EndCombo();
//        }
//
//        ImGui::Checkbox("Enabled", &pointLights[pointComboItem].m_enabled);
//        ImGui::Checkbox("Billboard", &pointLights[pointComboItem].m_draw_billboard);
//        ImGui::DragFloat3("Position", (float*)&pointLights[pointComboItem].m_pos, 0.1f);
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Ambient", (float*)&pointLights[pointComboItem].m_ambient);
//        ImGui::ColorEdit3("Diffuse", (float*)&pointLights[pointComboItem].m_diffuse);
//        ImGui::ColorEdit3("Specular", (float*)&pointLights[pointComboItem].m_specular);
//
//        ImGui::Separator();
//        ImGui::InputFloat("Constant", (float*)&pointLights[pointComboItem].m_constant, 0.1f, 1.0f);
//        ImGui::InputFloat("Linear", (float*)&pointLights[pointComboItem].m_linear, 0.01f, 0.1f);
//        ImGui::InputFloat("Quadratic", (float*)&pointLights[pointComboItem].m_quad, 0.001f, 0.01f);
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Color", (float*)&pointLights[pointComboItem].m_color);
//
//        ImGui::Separator();
//    }
//
//    if (ImGui::CollapsingHeader("Spot Lights"))
//    {
//        static int spotComboItem = 0;
//
//        if (ImGui::BeginCombo("Source", spotLights[spotComboItem]))
//        {
//            for (int i = 0; i < spotLights.size(); i++)
//            {
//                const bool is_selected = (spotComboItem == i);
//                if (ImGui::Selectable(spotLights[i], is_selected))
//                    spotComboItem = i;
//
//                if (is_selected)
//                    ImGui::SetItemDefaultFocus();
//            }
//            ImGui::EndCombo();
//        }
//
//        ImGui::Checkbox("Enabled", &spotLights[spotComboItem].m_enabled);
//        ImGui::Checkbox("Billboard", &spotLights[spotComboItem].m_draw_billboard);
//
//        if (spotLights[spotComboItem].m_name != "flashlight")
//        {
//            ImGui::DragFloat3("Position", (float*)&spotLights[spotComboItem].m_pos, 0.1f);
//            ImGui::SliderFloat3("Direction", (float*)&spotLights[spotComboItem].m_dir, -1.0f, 1.0f);
//        }
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Ambient", (float*)&spotLights[spotComboItem].m_ambient);
//        ImGui::ColorEdit3("Diffuse", (float*)&spotLights[spotComboItem].m_diffuse);
//        ImGui::ColorEdit3("Specular", (float*)&spotLights[spotComboItem].m_specular);
//
//        ImGui::Separator();
//        ImGui::InputFloat("Constant", (float*)&spotLights[spotComboItem].m_constant, 0.1f, 1.0f);
//        ImGui::InputFloat("Linear", (float*)&spotLights[spotComboItem].m_linear, 0.01f, 0.1f);
//        ImGui::InputFloat("Quadratic", (float*)&spotLights[spotComboItem].m_quad, 0.001f, 0.01f);
//
//        ImGui::Separator();
//        ImGui::DragFloat("Inner Angle", (float*)&spotLights[spotComboItem].m_innerCutoff, 0.5f, 0.0f, 90.0f);
//        ImGui::DragFloat("Outer Angle", (float*)&spotLights[spotComboItem].m_outerCutoff, 0.5f, 0.0f, 90.0f);
//
//        ImGui::Separator();
//        ImGui::ColorEdit3("Color", (float*)&spotLights[spotComboItem].m_color);
//
//        ImGui::Separator();
//    }
//
//    ImGui::SeparatorText("Material");
//    ImGui::SliderFloat("Material Shininess", (float*)&materialShininess, 1.0f, 256.0f);
//
//    ImGui::SeparatorText("Screen");
//    ImGui::SliderFloat("FOV", &FOV, 0.0f, 120.0f);
//
//    ImGui::Text("Cam Position: X %.3f Y %.3f Z %.3f", cam.m_pos.x, cam.m_pos.y, cam.m_pos.z);
//    ImGui::Text("Cam Orientation: X %.3f Y %.3f Z %.3f", cam.m_orientation.x, cam.m_orientation.y, cam.m_orientation.z);
//
//    ImGui::Text("%.3f ms (%.1f FPS)", deltaTime * 1000.0f, 1.0f / deltaTime);
//
//    ImGui::End();
//}