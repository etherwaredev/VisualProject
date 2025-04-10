#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"
#include "Headers/cglm/cglm.h"
#include "Headers/cglm/struct.h"
#include "Headers/VP_BuildShader.h"

#define VP_INIT_WIN_SIZE_H 900
#define VP_INIT_WIN_SIZE_W 1400

// framebuffer_size_callback was defined here due to clang complaining. Function is fine if moved right above glfwSetFramebufferSizeCallback()
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void VP_ReadInputKeyboard(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void){
    // Section: GLFW Init Block
	if (!glfwInit()) {printf("GLFW Initialization Failed\n"); return EXIT_FAILURE;}
	else {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //4
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //6
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	// Section: Window Creation
	GLFWwindow *window = glfwCreateWindow(VP_INIT_WIN_SIZE_W, VP_INIT_WIN_SIZE_H, "Visual Project", NULL, NULL);
	if (!window) {
		glfwTerminate();
		printf("GLFW Window Creation Failed\n"); return EXIT_FAILURE;
	}

	// Section: Window Selection & Setup
	glfwMakeContextCurrent(window);
	glViewport(0, 0, VP_INIT_WIN_SIZE_W, VP_INIT_WIN_SIZE_H);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Section: GLEW Init Block // MUST BE WITHIN OPENGL CONTEXT
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("GLEW Initialization Failed\n"); return EXIT_FAILURE;
	}

	// Section: Shader Loading & Compilation
	unsigned int VFProgram;
	VP_VFCompile(&VFProgram);


	// Section: File Based Vertices
	int VertSize;
	float* Vertices = VP_LoadVox("./Objects/Garden.vpvox", &VertSize);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertSize/*sizeof(Vertices)*/, Vertices, GL_STATIC_DRAW); // Alt Size Arg: sizeof(vertices)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); //ALT 6 ARG: (void*)0
	glEnableVertexAttribArray(0);

	/* Texture Mapping Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	// glBindBuffer(GL_ARRAY_BUFFER, 0); // You may optionally unbind if needed here as glVertexAttribPointer() bound/selected to the VBO.
	glBindVertexArray(0); // (Semi-Optional) VAO Unbind

	/*
	// Section: Temp Texture Loading
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int TexWidth, TexHeight, nrChannels; //If Loading Many, make this an array lol
	unsigned char *LoadedImage = stbi_load("Textures/Blueberry.jpg", &TexWidth, &TexHeight, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexWidth, TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, LoadedImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(LoadedImage); // Equivalent to free()
	LoadedImage = NULL; // STBI_IMAGE_FREE() DOES NOT SET THE * TO NULL.

	glUseProgram(VFProgram);
	glUniform1i(glGetUniformLocation(VFProgram, "InputTex0"), 0);
	*/

	// Section: Matrices
	//	// Sub-Section: View Matrix
	mat4 ViewMatrix; glm_mat4_identity(ViewMatrix);
	glm_translate_x(ViewMatrix, -.7);
	glm_translate_y(ViewMatrix, -.2);
	glm_translate_z(ViewMatrix, -2);
	// glm_mat4_print(ViewMatrix, stdout); // TMP DEBUG PRINT

	glUseProgram(VFProgram);
	glUniformMatrix4fv(glGetUniformLocation(VFProgram, "ViewMatrix"), 1, false, (const float *)&ViewMatrix);

	//	// Sub-Section: Projection Matrix
	mat4 ProjMatrix;
	glm_perspective(glm_rad(90.0f), ((float)VP_INIT_WIN_SIZE_W/(float)VP_INIT_WIN_SIZE_H), 0.1f, 100.0f, (vec4 *)&ProjMatrix);
	// glm_mat4_print(ProjMatrix, stdout); // TMP DEBUG PRINT

	glUseProgram(VFProgram);
	glUniformMatrix4fv(glGetUniformLocation(VFProgram, "ProjMatrix"), 1, false, (const float *)&ProjMatrix);

	// Section: Rendering Configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe Mode

	// Section: Main Frame Loop
	while (!glfwWindowShouldClose(window)){
		VP_ReadInputKeyboard(window);

		// Sub-Section: GL
		glClearColor(.25f, .08f, .37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture0);

		glUseProgram(VFProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);

		// Sub-Section: Frame End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	//free(Vertices);
	return EXIT_SUCCESS;
}
