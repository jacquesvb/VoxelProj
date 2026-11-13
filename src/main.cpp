#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <stb_image.h>
#include "Texture.h"
#include "Camera.h"

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "resources/"
#endif

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main()
{

	GLfloat vertices[] = {
		-0.5f,  0.0f,  0.5f, 0.83f, 0.7f, 0.44f, 0.0f, 0.0f,
		-0.5f,  0.0f, -0.5f, 0.83f, 0.7f, 0.44f, 0.0f, 1.0f,
		 0.5f,  0.0f, -0.5f, 0.83f, 0.7f, 0.44f, 1.0f, 1.0f,
		 0.5f,  0.0f,  0.5f, 0.83f, 0.7f, 0.44f, 1.0f, 0.0f,
		 0.0f,  0.8f,  0.0f, 0.83f, 0.7f, 0.44f, 0.5f, 0.5f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	gladLoadGL();

	try
	{
		Shader shaderProgram(RESOURCES_PATH "default.vert", RESOURCES_PATH "default.frag");

		Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

		Texture brickTex(RESOURCES_PATH "brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
		brickTex.texUnit(shaderProgram, "tex0");

		glEnable(GL_DEPTH_TEST);

		VAO VAO1;
		VAO1.Bind();

		VBO VBO1(vertices, sizeof(vertices));
		EBO EBO1(indices, sizeof(indices));

		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		while(!glfwWindowShouldClose(window))
		{
			// The window can be resized at any time (Hyprland tiles new windows),
			// so keep the viewport and camera aspect ratio in sync with it.
			int fbWidth, fbHeight;
			glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
			glfwGetWindowSize(window, &camera.width, &camera.height);
			if (fbWidth == 0 || fbHeight == 0 || camera.width == 0 || camera.height == 0)
			{
				// Minimized: nothing to draw, but keep the window responsive
				glfwPollEvents();
				continue;
			}
			glViewport(0, 0, fbWidth, fbHeight);

			// Specify the color of the background
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			// Clean the color and depth buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Tell OpenGL which Shader Program we want to use
			shaderProgram.Activate();

			camera.Inputs(window);
			camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

			VAO1.Bind();
			brickTex.Bind();
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// Delete GL objects while the context still exists, then tear down GLFW
		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
		brickTex.Delete();

		shaderProgram.Delete();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
