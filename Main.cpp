#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Header_Files/ShaderClass.h"
#include "Header_Files/VAO.h"
#include "Header_Files/VBO.h"
#include "Header_Files/EBO.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 800;

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f
};

GLuint indices[] = {
	0, 2, 1,
	0, 3, 2
};

int main(void) {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "lluanllax-Engine", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Shader shaderProgram("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
