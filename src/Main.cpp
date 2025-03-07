#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Header_Files/Mesh.h"
#include "Header_Files/LightManager.h"
#include "Header_Files/UIManager.h"


int SCR_WIDTH = 1920;
int SCR_HEIGHT = 1080;

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

bool RayIntersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& sphereCenter, float sphereRadius) {
    glm::vec3 oc = rayOrigin - sphereCenter;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(oc, rayDirection);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

int main() {

    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using 
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "Render_Engine"
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Render_Engine", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Texture texture[]{
        Texture("Resources/planks.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("Resources/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(texture, texture + sizeof(texture) / sizeof(Texture));
    Mesh floor(verts, ind, tex);

    Shader lightShader("Shaders/lightVertex.glsl", "Shaders/lightFragment.glsl");

    LightManager lightManager;
    UIManager uiManager(lightManager);

    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main while loop
    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera.Inputs(window);
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

        // Update light positions and colors in shaders
        for (const auto& light : lightManager.GetLights()) {
            lightShader.Activate();
            glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.model));
            glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), light.color.x, light.color.y, light.color.z, light.color.w);
            shaderProgram.Activate();
            glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.position.x, light.position.y, light.position.z);
            glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.color.x, light.color.y, light.color.z, light.color.w);
        }

        floor.Draw(shaderProgram, camera);

        for (const auto& light : lightManager.GetLights()) {
            light.mesh->Draw(lightShader, camera);
        }

        // Detectar clic del ratón
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glm::vec3 rayOrigin = camera.Position;
            glm::vec3 rayDirection = camera.GetRayFromMouse(window);

            for (const auto& light : lightManager.GetLights()) {
                if (RayIntersectsSphere(rayOrigin, rayDirection, light.position, 0.1f)) {
                    uiManager.selectedObjectId = light.id;
                    break;
                }
            }
        }

        uiManager.Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    shaderProgram.Delete();
    lightShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}