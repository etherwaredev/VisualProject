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

#define VP_INIT_WIN_SIZE_H 950
#define VP_INIT_WIN_SIZE_W 1500

// Note: framebuffer_size_callback was defined here due to clang complaining. Function is fine if moved right above glfwSetFramebufferSizeCallback()
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void VP_ReadInputKeyboard(GLFWwindow *window, vec3 EyeVec, vec3 CenterVec, vec3 UpVec, mat4 VMatrix){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm_vec3_sub(CenterVec, EyeVec, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm_vec3_add(CenterVec, EyeVec, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		vec3 TMPVecKeyA;
		glm_cross(EyeVec, UpVec, TMPVecKeyA);
		glm_normalize(TMPVecKeyA);
		glm_vec3_add(CenterVec, TMPVecKeyA, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		vec3 TMPVecKeyD;
		glm_cross(EyeVec, UpVec, TMPVecKeyD);
		glm_normalize(TMPVecKeyD);
		glm_vec3_sub(CenterVec, TMPVecKeyD, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glm_vec3_add(CenterVec, UpVec, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		glm_vec3_sub(CenterVec, UpVec, CenterVec);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vec3 TMPVecKeyUP;
		glm_cross(UpVec, EyeVec, TMPVecKeyUP);
		glm_vec3_rotate(EyeVec, 0.05f, TMPVecKeyUP);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		vec3 TMPVecKeyDOWN;
		glm_cross(EyeVec, UpVec, TMPVecKeyDOWN);
		glm_vec3_rotate(EyeVec, 0.05f, TMPVecKeyDOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		glm_vec3_rotate(EyeVec, 0.05f, UpVec);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		glm_vec3_rotate(EyeVec, -0.05f, UpVec);
	}
}

int main(void){
    // Section: GLFW Init Block
    #if defined(__linux__)
    	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    #endif

	if (!glfwInit()) {printf("GLFW Initialization Failed\n"); return EXIT_FAILURE;}
	else {
		#if defined(__linux__)
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //6
		#endif

		#if defined(_WIN32)
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //6
		#endif

		#if defined(__APPLE__)
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //1
		#endif

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
	GLenum GlewErrCheck = glewInit();
	if (GLEW_OK != GlewErrCheck) {
		printf("GLEW Initialization Failed\n"); return EXIT_FAILURE;
	}

	// Section: Shader Loading & Compilation
	unsigned int CoreShaderProgram;
	VP_CompileShaders(&CoreShaderProgram, "./Shaders/Shader.vert", "./Shaders/Shader.geo", "./Shaders/Shader.frag");

	unsigned int LightingShaderProgram;
	VP_CompileShaders(&LightingShaderProgram, "./Shaders/Lighting/LShader.vert", "./Shaders/Lighting/LShader.geo", "./Shaders/Lighting/LShader.frag");

	// Section: File Based Vertices
	int VertSize;
	float* Vertices = VP_LoadVox("./Objects/Garden.vpvox", &VertSize);
	if (!Vertices) {
		printf("VERTICES COULD NOT BE LOADED\n");
		return EXIT_FAILURE;
	}

	int LoadedObjects = (VertSize/24); // Every 3 Vertex Components == 24 bytes

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertSize, Vertices, GL_STATIC_DRAW); // Alt Size Arg: sizeof(vertices)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); //ALT 6 ARG: (void*)0
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); // VAO Unbind

	// Section: Light (Partially Decorative)
	float LightVertices[] = {
		0.0f, 5.0f, 0.0f,
	}; // Note: Value fed directly to a uniform to main shader

	GLuint LightPointVBO, LightPointVAO;
	glGenVertexArrays(1, &LightPointVAO);
	glGenBuffers(1, &LightPointVBO);

	glBindVertexArray(LightPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, LightPointVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(LightVertices), &LightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); //ALT 6 ARG: (void*)0
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); // VAO Unbind

	/* Texture Mapping Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	// glBindBuffer(GL_ARRAY_BUFFER, 0); // Note: You may optionally unbind if needed here as glVertexAttribPointer() bound/selected to the VBO.

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

	int TexWidth, TexHeight, nrChannels; // If Loading Many, make this an array
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
	vec3 VMEyeVector = {0.0, 0.0, 1.0};
	vec3 VMCenterVector = {0.0, 0.0, 0.0};
	vec3 VMUpVector = {0.0, 1.0, 0.0};

	vec3 VMTempEyeVec;

	glUseProgram(CoreShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(CoreShaderProgram, "ViewMatrix"), 1, false, (const float *)&ViewMatrix);

	//	// Sub-Section: Projection Matrix
	mat4 ProjMatrix;
	glm_perspective(glm_rad(90.0f), ((float)VP_INIT_WIN_SIZE_W/(float)VP_INIT_WIN_SIZE_H), 0.1f, 100.0f, (vec4 *)&ProjMatrix);

	glUseProgram(CoreShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(CoreShaderProgram, "ProjMatrix"), 1, false, (const float *)&ProjMatrix);

	// Section: UBO
	GLuint MainUBO;
	glGenBuffers(1, &MainUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, MainUBO);
	glBufferData(GL_UNIFORM_BUFFER, (sizeof(mat4)*3), NULL, GL_DYNAMIC_DRAW);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, MainUBO, 0, (sizeof(mat4)*3));
	glUniformBlockBinding(CoreShaderProgram, glGetUniformBlockIndex(CoreShaderProgram, "UniMat"), 0);
	glUniformBlockBinding(LightingShaderProgram, glGetUniformBlockIndex(CoreShaderProgram, "UniMat"), 0);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &ProjMatrix);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &ViewMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Section: Rendering Configuration
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe Mode
	glEnable(GL_DEPTH_TEST);

	// Section: Misc (LightVertUniform)
	glUniform3fv(glGetUniformLocation(CoreShaderProgram, "LightPos"), 1, (const float *)&LightVertices);

	// Section: Main Frame Loop
	while (!glfwWindowShouldClose(window)){
		VP_ReadInputKeyboard(window, VMEyeVector, VMCenterVector, VMUpVector, ViewMatrix);

		// Sub-Section: Background
		glClearColor(.10f, .04f, .17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Texture Related Calls
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		*/

		// Sub-Section: Movement (View Matrix Manipulation)
		glm_vec3_add(VMCenterVector, VMEyeVector, VMTempEyeVec);
		glm_lookat(VMTempEyeVec, VMCenterVector, VMUpVector, ViewMatrix);

		glUseProgram(CoreShaderProgram);
		glBindBuffer(GL_UNIFORM_BUFFER, MainUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &ViewMatrix);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glUniform3fv(glGetUniformLocation(CoreShaderProgram, "CameraPos"), 1, (const float *)&VMCenterVector);

		// Sub-Section: Drawing
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, LoadedObjects);

		glUseProgram(0);
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);

		glUseProgram(LightingShaderProgram);
		glBindVertexArray(LightPointVAO);
		glDrawArrays(GL_POINTS, 0, (sizeof(LightVertices)/12));

		// Sub-Section: Frame End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	free(Vertices);
	return EXIT_SUCCESS;
}